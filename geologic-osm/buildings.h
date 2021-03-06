#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <QtOpenGL/QtOpenGL>

class Buildings {
    OGRMultiLineString _bld_borders;
    OGRSpatialReference _georef;

    GLuint _display_list = 0;
    GLuint _surface_list = 0;
    OGRSpatialReference *_viewref;

public:
    Buildings(OGRSpatialReference * v);
    void load(const char *filename);
    void draw();
    void draw(unsigned long long t);
};

#endif // BUILDINGS_H
