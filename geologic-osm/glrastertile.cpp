#include "glrastertile.h"
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <QImage>

using namespace std;

GLRasterTile::GLRasterTile() {
}

GLRasterTile::~GLRasterTile() {
    clear(*this);
}

void clear(GLRasterTile &me) {
    if (me.textr != nullptr && me.textr->textureId() != 0)
        me.textr->destroy();
    if (me.list_index != 0)
        glDeleteLists(me.list_index, 1);
}

void init(GLRasterTile &me, int x, int y, int zoom) {
    double x_min = geoMetrics::tile2utm(x, zoom);
    double y_min = geoMetrics::tile2utm(y + 1, zoom);
    double x_max = geoMetrics::tile2utm(x + 1, zoom);
    double y_max = geoMetrics::tile2utm(y, zoom);
    double x_min_wgs84 = geoMetrics::utm2lon(x_min);
    double y_min_wgs84 = geoMetrics::utm2lat(y_min);
    double x_max_wgs84 = geoMetrics::utm2lon(x_max);
    double y_max_wgs84 = geoMetrics::utm2lat(y_max);
    me.corners_dataref[0] = {x_min_wgs84, y_min_wgs84};
    me.corners_dataref[1] = {x_max_wgs84, y_min_wgs84};
    me.corners_dataref[2] = {x_max_wgs84, y_max_wgs84};
    me.corners_dataref[3] = {x_min_wgs84, y_max_wgs84};
}

string getFilename(GLRasterTile &me, int x, int y, int zoom) {
    char filen[100];
    sprintf(filen, "data/%d-%d-%d.jpg", zoom, x, y);
    return string(filen);
}

string getUrl(GLRasterTile &me, int x, int y, int zoom, const std::string &url) {
    char curl[300];
    sprintf(curl, url.c_str(), zoom, x, y);
    return string(curl);
}

void load(GLRasterTile &me, const string &filename) {
    me._tex_file = filename;
}

void draw(GLRasterTile &me) {
    if (me.textr == nullptr) {
        me.textr = make_unique<QOpenGLTexture>(QImage(me._tex_file.c_str()).mirrored());
        me.textr->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        me.textr->setMagnificationFilter(QOpenGLTexture::Linear);
    }
    if (me.textr != nullptr && me.textr->textureId() != 0) {
        if (me.list_index == 0) {
            if ((me.list_index = glGenLists(1)) == 0)
                return;
            glNewList(me.list_index, GL_COMPILE_AND_EXECUTE);
            me.textr->bind();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0);
                glVertex3d(me.corners_viewref[0].getX(), me.corners_viewref[0].getY(), -0.1);
                glTexCoord2f(1.0, 0.0);
                glVertex3d(me.corners_viewref[1].getX(), me.corners_viewref[1].getY(), -0.1);
                glTexCoord2f(1.0, 1.0);
                glVertex3d(me.corners_viewref[2].getX(), me.corners_viewref[2].getY(), -0.1);
                glTexCoord2f(0.0, 1.0);
                glVertex3d(me.corners_viewref[3].getX(), me.corners_viewref[3].getY(), -0.1);
            glEnd();
            me.textr->release();
            glEndList();
        }
        if (me.list_index != 0) {
            glCallList(me.list_index);
        }
    }
}

void convert(GLRasterTile &me, OGRSpatialReference *&from, OGRSpatialReference *&to) {
    for (int i=0; i<4; ++i) {
        me.corners_dataref[i].assignSpatialReference(from);
        me.corners_viewref[i] = me.corners_dataref[i];
        me.corners_viewref[i].transformTo(to);
    }
}
