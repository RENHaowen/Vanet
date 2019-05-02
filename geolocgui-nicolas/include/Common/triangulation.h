#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <iostream>
#include <vector>
#include <ogr_geometry.h>
#include <ogr_spatialref.h>
#include <memory>

class Triangulation {
    struct Point {
        double x;
        double y;
        double z;
    };

    struct Triangle_test {
        Point p1, p2, p3;
    };

    friend bool operator==(const Point &p1, const Point &p2);
    friend bool operator!=(const Point &p1, const Point &p2);
    friend std::ostream &operator<<(std::ostream &os, const Point &p);
    friend std::ostream &operator<<(std::ostream &os, const Triangle_test &triangle);

    std::vector<Point> polygon;
    std::vector<Triangle_test> triangles;

    friend double raw_angle(const Point &A, const Point &B, const Point &C);
    friend double angle(const Point &A, const Point &B, const Point &C);
    friend double distance(const Point &p1, const Point &p2);
    friend double sqr_distance(const Point &p1, const Point &p2);
    friend bool point_in_triangle(const Point &corner1, const Point &corner2, const Point &corner3, const Point &p);
    friend bool point_in_triangle(const Triangle_test &triangle, const Point &p);

public:
    using TriangulationType = enum {TRIANGULATE_EAR};

    Triangulation(const OGRPolygon &poly);
    Triangulation(const OGRPolygon &poly, OGRCoordinateTransformation *t);
    std::vector<Triangle_test> triangulate_ear();
    std::unique_ptr<OGRGeometryCollection> triangulate(TriangulationType type);
    ~Triangulation() = default;
};

#endif // TRIANGULATION_H
