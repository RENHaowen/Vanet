#include "glvectortile.h"
#include <iostream>
#include <tagsbuilder.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <OpenGL/linedisplayer.h>

using namespace std;

GLVectorTile::GLVectorTile() {
}

void init(GLVectorTile &me, int x, int y, int zoom) {
}

string getFilename(GLVectorTile &me, int x, int y, int zoom) {
    char filen[100];
    sprintf(filen, "data/%d-%d-%d.osm", zoom, x, y);
    return string(filen);
}

string getUrl(GLVectorTile &me, int x, int y, int zoom, const std::string &url) {
    double bbox[4];

    bbox[1] = geoMetrics::utm2lon(geoMetrics::tile2utm(x, zoom));
    bbox[0] = geoMetrics::utm2lat(geoMetrics::tile2utm(y + 1, zoom));
    bbox[3] = geoMetrics::utm2lon(geoMetrics::tile2utm(x + 1, zoom));
    bbox[2] = geoMetrics::utm2lat(geoMetrics::tile2utm(y, zoom));
    TagsBuilder tb(url);
    tb.setBoudingBox(bbox);
    tb.setoutputformat("xml");
    tb.is("highway");

//    cout << tb.toString() << endl;
    return tb.toString();
}

void load(GLVectorTile &me, const string &filename) {
    me._roads = make_unique<Roads>();
    me._roads->setLoader(new OSMRouteLoader(nullptr));
    me._roads->load(filename.c_str());
}

void draw(GLVectorTile &me) {
    Color c{1.0, 1.0, 1.0, 1.0};
    if (me._list_index == 0) {
        if ((me._list_index = glGenLists(1)) == 0)
            return;
        const auto &mEdge = me._roads->getmEdge();
        glNewList(me._list_index, GL_COMPILE_AND_EXECUTE);
        for (const auto &edge: mEdge) {
            LineDisplayer::draw(edge.second->getNodes(), &c, 1.0, me._viewref);
        }
        glEndList();
        me._roads.reset();
    } else {
        glColor4d(c.red, c.green, c.blue, c.alpha);
        glCallList(me._list_index);
    }
}

void convert(GLVectorTile &me, OGRSpatialReference *&from, OGRSpatialReference *&to) {
    me._dataref = from;
    me._viewref = to;
    auto &mEdge = me._roads->getmEdge();
//    cout << "Convert " << mEdge.size() << " edges" << endl;
    OGRCoordinateTransformation *t = OGRCreateCoordinateTransformation(from, to);
    if (t == nullptr) {
        return;
    }
    for (auto &edge: mEdge) {
        edge.second->getNodes()->transform(t);
    }
}

void clear(GLVectorTile &me) {
    if (me._list_index != 0)
        glDeleteLists(me._list_index, 1);
}
