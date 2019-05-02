#ifndef GLRASTERTILE_H
#define GLRASTERTILE_H

#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <QtOpenGL/QtOpenGL>
#include <memory>
#include <string>

class GLRasterTile {
    OGRPoint corners_dataref[4];
    OGRPoint corners_viewref[4];
    OGRSpatialReference *dataref;
    OGRSpatialReference *viewref;
    std::unique_ptr<QOpenGLTexture> textr;
    GLuint list_index = 0;
    std::string _tex_file;

public:
    GLRasterTile();
    ~GLRasterTile();
    friend void init(GLRasterTile &me, int x, int y, int zoom);
    friend std::string getFilename(GLRasterTile &me, int x, int y, int zoom);
    friend std::string getUrl(GLRasterTile &me, int x, int y, int zoom, const std::string &url);
    friend void load(GLRasterTile &me, const std::string &filename);
    friend void draw(GLRasterTile &me);
    friend void convert(GLRasterTile &me, OGRSpatialReference *&from, OGRSpatialReference *&to);
    friend void clear(GLRasterTile &me);
};

#endif // GLRASTERTILE_H
