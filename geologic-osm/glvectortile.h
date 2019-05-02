#ifndef GLVECTORTILE_H
#define GLVECTORTILE_H

#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <QtOpenGL/QtOpenGL>
#include <memory>
#include <string>
#include <geologic-core/objects/urban/roads.h>
#include <osmrouteloader.h>

class GLVectorTile {
    OGRSpatialReference *_dataref;
    OGRSpatialReference *_viewref;
    GLuint _list_index = 0;
    std::unique_ptr<Roads> _roads;

public:
    GLVectorTile();
    ~GLVectorTile() {}
    friend void init(GLVectorTile &me, int x, int y, int zoom);
    friend std::string getFilename(GLVectorTile &me, int x, int y, int zoom);
    friend std::string getUrl(GLVectorTile &me, int x, int y, int zoom, const std::string &url);
    friend void load(GLVectorTile &me, const std::string &filename);
    friend void draw(GLVectorTile &me);
    friend void convert(GLVectorTile &me, OGRSpatialReference *&from, OGRSpatialReference *&to);
    friend void clear(GLVectorTile &me);
};

#endif // GLVECTORTILE_H
