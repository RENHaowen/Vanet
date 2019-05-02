#ifndef MOVABLEVIEW_H
#define MOVABLEVIEW_H

#include <Scene/view.h>
#include <Scene/layer.h>
#include <memory>
#include <iostream>
#include <vector>
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
#include <geologic-core/objects/geo/untemporal/Bbox.h>
#include <data/datamanager.h>

///*!
// * \brief BoundingBox defines a bounding box by:
// * x_min, y_min, x_max, y_max
// */
//struct BoundingBox {
//    double x_min = 0.0;
//    double y_min = 0.0;
//    double x_max = 0.0;
//    double y_max = 0.0;

//public:
//    BoundingBox() = default;
//    BoundingBox(double xmin, double ymin, double xmax, double ymax): x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) {}
//    ~BoundingBox() = default;

//    BoundingBox &operator=(const std::tuple<double, double, double, double> &bbox) {
//        x_min = std::get<0>(bbox);
//        x_max = std::get<2>(bbox);
//        y_min = std::get<1>(bbox);
//        y_max = std::get<3>(bbox);
//        return *this;
//    }

//    bool operator!=(const BoundingBox &tbb) const {
//        return tbb.x_min!=x_min && tbb.x_max!=x_max && tbb.y_min!=y_min && tbb.y_max!=y_max;
//    }
//};

//typedef std::tuple<double, double, double, double> BoundingBox;

/*!
 * \class MovableView
 * \brief Class MovableView is an implementation of interface View.
 * A MovableView is a view in which the user can navigate.
 * This kind of view is used for instance for the main geographical view.
 * It provides especially the following features:
 * - zoom,
 * - rotation around x-axis,
 * - translation through mouse left button dragging.
 *
 * \see View
 */
class MovableView: public View {
private:
    std::vector<std::unique_ptr<Layer>> vLayer;     ///< Contains the layers included in the view
    Bbox view_bbox;                                 ///< View bounding box in data SRS
    Bbox data_bbox;                                 ///< View bounding box in view SRS
    OGRPoint center_dataref;                        ///< Coordinates where the user actually looks at, in data SRS
    OGRPoint center_viewref;                        ///< Coordinates where the user actually looks at, in view SRS
    double z_clipping_min, z_clipping_max;          ///< Z Clipping plans (eventually to be computed)
    int zoom;                                       ///< Used for zoom, based on OSM zoom levels
    const int max_scale = 18;                       ///< Zoom limit (18 for now)
    double x_rotation;                              ///< View X rotation (from 0.0 to 75.0)
    double z_rotation;                              ///< View Z rotation (no limit)

public:

    /*!
     * \brief Sets current View view center (i.e. where the user is looking at)
     * \param x in viewRef SRS
     * \param y in viewRef SRS
     */
    //virtual void setViewCenter(double x, double y);

    /*!
     * \brief Moves the current View center from a relative amount.
     * Updates the View center based on a relative offset.
     * \param x_offset the horizontal offset in viewRef SRS.
     * \param y_offset the vertical offset in viewRef SRS.
     */
    virtual void moveViewCenter(double x_offset, double y_offset);

    virtual void moveViewCenter(int x_offset_px, int y_offset_px);

    /*!
     * \brief setCenter modifies the view center.
     * In particular, it sets the center in both data and view references,
     * and recomputes the bounding box parameters.
     * \param p is the new center.
     */
    virtual void setCenter(const OGRPoint &p);

    MovableView(): View() {}

    /*!
     * \brief Destructor.
     * Does not do anything currently.
     */
    ~MovableView();

    virtual void setZoom(int z) {zoom = z; resize();}
    virtual int getZoom() {return zoom;}

    /*!
     * \brief Called when a zoom in action is triggered in GLDisplay.
     */
    virtual void zoomIn();

    /*!
     * \brief Called when a zoom out action is triggered in GLDisplay.
     */
    virtual void zoomOut();

    /*!
     * \brief Inits the View GL state.
     */
    virtual void initView();

    /*!
     * \brief Computes the 2D GL Ortho boundaries.
     * It is used to compute the boundaries without changing the image aspect ratio.
     * It uses footprint size and screen size to compute the boundaries.
     */
    virtual void resize();

    /*!
     * \brief Gets the minimum values of View boundaries.
     * \return x and y minimum value as a std::pair of double.
     */
    virtual std::pair<double, double> getMinBoundaries();

    double getViewXMin() {return view_bbox.minx;}
    double getViewXMax() {return view_bbox.maxx;}
    double getViewYMin() {return view_bbox.miny;}
    double getViewYMax() {return view_bbox.maxy;}

    double getDataXMin() {return data_bbox.minx;}
    double getDataXMax() {return data_bbox.maxx;}
    double getDataYMin() {return data_bbox.miny;}
    double getDataYMax() {return data_bbox.maxy;}

    Bbox &getDataBBox() {return data_bbox;}
    Bbox &getViewBBox() {return view_bbox;}

    const OGRPoint &getCenterInDataRef() const { return center_dataref; }
    const OGRPoint &getCenterInViewRef() const { return center_viewref; }

    void setGeoRefs();
    const std::vector<std::unique_ptr<Layer>> &getVLayers() {return vLayer;}
    void addLayer(std::unique_ptr<Layer> l) { vLayer.push_back(move(l)); }

    void registerManager(DataManager* manager);
    void hoverTrackSignal(double posX, double posY);
    void hoverClickSignal(double posX, double posY);
    void hoverReleaseSignal();

    DataManager* manager = nullptr;

    /*!
     * \brief Draws the View when no animation is playing.
     * This method is called by the parent GLDisplay when it requires to refresh itself.
     * Does not do anything if attribute visible is set to false.
     */
    virtual void draw();

    /*!
     * \brief Draws the View when animation is playing.
     * This method is called by the parent GLDisplay when it plays an animation.
     * Does not do anything if attribute visible is set to false.
     * \param t The current simulation time (seconds since last midnight).
     */
    virtual void draw(unsigned long long t);


    double getXrotation() {return x_rotation;}
    double getZrotation() {return z_rotation;}
    virtual void reinitializeAngles() {x_rotation = z_rotation = 0.0;}
    /*!
     * \brief Modifies the View angle.
     * It modifies the View rotation angle.
     * \param mod_angle The new rotation angle value.
     */
    virtual void modAngle(double mod_angle);

    /*!
     * \brief Modifies the View angle.
     * It modifies the View rotation angle.
     * \param mod_angle The new rotation angle value.
     */
    virtual void modZAngle(double mod_angle);

    /**/
};

#endif // MOVABLEVIEW_H
