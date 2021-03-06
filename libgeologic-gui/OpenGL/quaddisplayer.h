#ifndef QUADDISPLAYER_H
#define QUADDISPLAYER_H

#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <OpenGL/texture.h>
#include <string>

class QuadDisplayer {
public:
    QuadDisplayer();
    static void draw(OGRPoint corners[4], const std::string &texture_file, OGRSpatialReference *to);
};

#endif // QUADDISPLAYER_H
