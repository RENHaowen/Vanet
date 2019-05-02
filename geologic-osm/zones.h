#ifndef ZONES_H
#define ZONES_H

#include <osmloader.h>
#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <vector>
#include <Common/triangle.h>
#include <data/data.h>
#include <string>
#include <memory>
#include <QtOpenGL/QtOpenGL>

class Zones {
    OGRMultiLineString zones_boundaries;
    OGRSpatialReference geo_ref;
    OGRSpatialReference *_viewref;

public:
    Zones();

    /*!
     * \brief load loads an osm file with relations, way and nodes defining administrative zones
     * Goes through relations, finds ways of the outer bound, assembles them.
     * \param filename
     */
    void load(const char *filename);
    const OGRMultiLineString &getBoundaries() const {return zones_boundaries;}
    const OGRSpatialReference &getGeoRef() {return geo_ref;}
    OGRMultiLineString *getBoundariesPtr() {return &zones_boundaries;}
    OGRSpatialReference *getGeoRefPtr() {return &geo_ref;}
    OGRSpatialReference *getViewref() const;
    void setViewref(OGRSpatialReference *viewref);
};

class ZonesData: public Data {
    std::unique_ptr<Zones> _zones;
    GLuint display_list = 0;
    OGRSpatialReference *_viewref;

public:
    ZonesData(std::string &n, OGRSpatialReference *viewref): ZonesData(n.c_str(), viewref) {}
    ZonesData(const char *n, OGRSpatialReference *viewref): Data(n), _viewref(viewref) {_zones = std::make_unique<Zones>();}
    void load_from_file(const char *name);
    virtual ~ZonesData() {}
    virtual void draw();
    virtual void draw(unsigned long long simu_time) {draw();}
};

#endif // ZONES_H
