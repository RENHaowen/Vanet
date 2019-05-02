#ifndef TILEGRID_H
#define TILEGRID_H

#include <QNetworkProxy>
#include <Scene/movableview.h>
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <geologic-core/objects/geo/untemporal/Bbox.h>
#include <ogr_spatialref.h>
#include <data/data.h>
#include <data/tile.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <QtOpenGL/QtOpenGL>

///*!
// * \brief The TileBoundingBox struct is used to store the grid bounds as tile indices
// */
//struct TileBoundingBox {
//    int x_min = 0;
//    int y_min = 0;
//    int x_max = 0;
//    int y_max = 0;

//    TileBoundingBox() = default;
//    TileBoundingBox(int xmin, int ymin, int xmax, int ymax): x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) {}
//    ~TileBoundingBox() = default;
//    TileBoundingBox &operator=(const TileBoundingBox &other) = default;
//    TileBoundingBox &operator=(const std::tuple<int, int, int, int> &bbox) {
//        x_min = std::get<0>(bbox);
//        x_max = std::get<2>(bbox);
//        y_min = std::get<1>(bbox);
//        y_max = std::get<3>(bbox);
//        return *this;
//    }
//    bool operator!=(const TileBoundingBox &tbb) const {
//        return tbb.x_min!=x_min && tbb.x_max!=x_max && tbb.y_min!=y_min && tbb.y_max!=y_max;
//    }
//};

/*!
 * \class TileGrid manages a grid of tiles one one type.
 * TileGrid class is in charge of getting the screen footprint,
 * computing the tile indices covering the footprint, creating
 * the tiles, cleaning the tiles and update the list of tiles
 * required when the screen moves or resizes.
 */
template <typename T>
class TileGrid: public Data {

    std::vector<std::unique_ptr<Tile<T>>> _grid;    ///< The actuel content of the grid, one tile in each element
    QNetworkAccessManager *_network_mgr;            ///< Net mgr used to query data on the network, provided by the owner of TileGrid
    MovableView *_my_view;                          ///< view managing the tile grid, provided by the owner of TileGrid
    OGRSpatialReference *_dataref;                  ///< spatial reference in which data is stored, provided by the owner of TileGrid
    OGRSpatialReference *_viewref;                  ///< spatial reference in which data is displayed, provided by the owner of TileGrid
    Bbox _viewDataBBox;                             ///< Current view bounding box in data spatial reference, used to update the grid
    Bbox _currentTileBbox;                          ///< Current bounding box in tile coordinates
    std::string base_url;                           ///< Base URL to be passed to Tile<T>, then to T
    bool firstTime = true;

public:
    TileGrid() = delete;
    TileGrid(std::string& name, MovableView *v, OGRSpatialReference *d, OGRSpatialReference *vr, const std::string &url):
        TileGrid(name.c_str(), v, d, vr, url) {}

    TileGrid(const char *name, MovableView *v, OGRSpatialReference *d, OGRSpatialReference *vr, const std::string &url);

    virtual void draw();
    virtual void draw(unsigned long long t) {draw();}
    virtual ~TileGrid() {delete _network_mgr;}
};

template <typename T>
TileGrid<T>::TileGrid(const char *name, MovableView *v, OGRSpatialReference *d, OGRSpatialReference *vr, const std::__cxx11::string &url):
    Data(name), _my_view(v), _dataref(d), _viewref(vr), base_url(url) {
    _network_mgr = new QNetworkAccessManager;
    //_network_mgr->setProxy(QNetworkProxy(QNetworkProxy::DefaultProxy));
}

template <typename T>
void TileGrid<T>::draw() {
    //std::cout << "TileGrid::draw() ::";

    if (_my_view == nullptr || _network_mgr == nullptr || _dataref == nullptr || _viewref == nullptr)
        return;
    //std::cout << " start" << std::endl;
    Bbox viewBBox = _my_view->getDataBBox();
    if (viewBBox != _viewDataBBox) {

        _viewDataBBox = viewBBox;
        int tile_x_min = geoMetrics::long2tilex(viewBBox.minx, _my_view->getZoom());
        int tile_x_max = geoMetrics::long2tilex(viewBBox.maxx, _my_view->getZoom());
        int tile_y_min = geoMetrics::lat2tiley(viewBBox.miny, _my_view->getZoom());
        int tile_y_max = geoMetrics::lat2tiley(viewBBox.maxy, _my_view->getZoom());

        if (tile_y_min > tile_y_max) std::swap(tile_y_min, tile_y_max);

        if (tile_x_min != _currentTileBbox.minx || tile_x_max != _currentTileBbox.maxx ||
                tile_y_min != _currentTileBbox.miny || tile_y_max != _currentTileBbox.maxy) {
            std::vector<std::unique_ptr<Tile<T>>> oldTiles;
            std::swap(oldTiles, _grid);
            _currentTileBbox = std::make_tuple(tile_x_min, tile_y_min, tile_x_max, tile_y_max);
            for (int i=tile_x_min-1; i<=tile_x_max+1; ++i)
                for (int j=tile_y_min-1; j<=tile_y_max+1; ++j) {
                    int z = _my_view->getZoom();
                    auto it = std::find_if(begin(oldTiles), end(oldTiles), [i, j, z](const std::unique_ptr<Tile<T>> &ptr) {
                        return ptr->_x==i && ptr->_y==j && ptr->_zoom==z;
                    });
                    if (it == std::end(oldTiles)) {
                        _grid.push_back(std::make_unique<Tile<T>>(z, i, j, _dataref, _viewref, _network_mgr, base_url));
                    } else {
                        _grid.push_back(std::move(*it));
                        oldTiles.erase(it);
                    }
                }

            for (auto &tile: oldTiles) { /// TODO: pass to cleaner thread
                tile->clean();
            }
            oldTiles.clear();
        }
    }
    glEnable(GL_TEXTURE_2D);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    for (const auto &tile: _grid) {
        tile->display();
    }
    glDisable(GL_TEXTURE_2D);
    //std::cout << "TileGrid::draw() done" << std::endl;
}/**/

#endif // TILEGRID_H

