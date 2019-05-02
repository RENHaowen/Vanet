#include <Common/triangulation.h>
#include <geologic-core/utils/customToolbox.h>
#include <cmath>

using namespace std;

bool operator==(const Triangulation::Point &p1, const Triangulation::Point &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const Triangulation::Point &p1, const Triangulation::Point &p2) {
    return !(p1 == p2);
}

ostream &operator<<(ostream &os, const Triangulation::Point &p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

ostream &operator<<(ostream &os, const Triangulation::Triangle_test &triangle) {
    os << triangle.p1 << " - " << triangle.p2 << " - " << triangle.p3;
    return os;
}

Triangulation::Triangulation(const OGRPolygon &poly, OGRCoordinateTransformation *t) {
    OGRLinearRing *boundaries = reinterpret_cast<OGRLinearRing *>(poly.getExteriorRing()->clone());
    if (t != nullptr) {
        cout << "Tranform not null" << endl;
        boundaries->transform(t);
    }
    int count = boundaries->getNumPoints();
    for (int i=0; i<count; ++i) {
        OGRPoint p;
        boundaries->getPoint(i, &p);
        polygon.push_back({p.getX(), p.getY(), p.getZ()});
    }
    delete boundaries;
}

Triangulation::Triangulation(const OGRPolygon &poly) {
    const OGRLinearRing *boundaries = poly.getExteriorRing();
    int count = boundaries->getNumPoints();
    for (int i=0; i<count; ++i) {
        OGRPoint p;
        boundaries->getPoint(i, &p);
        polygon.push_back({p.getX(), p.getY(), p.getZ()});
    }
}

double raw_angle(const Triangulation::Point &A, const Triangulation::Point &B, const Triangulation::Point &C) {
    Triangulation::Point Ap{A.x-B.x, A.y-B.y, 0}, Cp{C.x-B.x, C.y-B.y, 0};
    double A_angle = atan(Ap.y / Ap.x);
    double C_angle = atan(Cp.y / Cp.x);
    if (Ap.x < 0.0)
        A_angle = M_PI + A_angle;
    if (Cp.x < 0.0)
        C_angle = M_PI + C_angle;

    return min(abs(C_angle - A_angle) * 180.0 / M_PI, 360.0 - abs(C_angle - A_angle) * 180.0 / M_PI);
}

// A, B, C must be counter clock wise
double angle(const Triangulation::Point &A, const Triangulation::Point &B, const Triangulation::Point &C) {
    Triangulation::Point Ap{A.x-B.x, A.y-B.y, 0}, Cp{C.x-B.x, C.y-B.y, 0};
    double A_angle = atan(Ap.y / Ap.x);
    double C_angle = atan(Cp.y / Cp.x);

    if (Ap.x < 0.0)
        A_angle = M_PI + A_angle;
    if (Cp.x < 0.0)
        C_angle = M_PI + C_angle;

    double agl = C_angle - A_angle;
    return (agl > 0.0 ? -2.0*M_PI+agl : agl) * 180.0 / M_PI;
}

double distance(const Triangulation::Point &p1, const Triangulation::Point &p2) {
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

double sqr_distance(const Triangulation::Point &p1, const Triangulation::Point &p2) {
    return (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);
}

bool point_in_triangle(const Triangulation::Point &corner1, const Triangulation::Point &corner2,
                       const Triangulation::Point &corner3, const Triangulation::Point &p) {
    if (corner1 == p || corner2 == p || corner3 == p)
        return false;
    double sum_angles = raw_angle(corner2, p, corner1) + raw_angle(corner3, p, corner2) + raw_angle(corner1, p, corner3);
    //cout << "Testing " << p << " in " << corner1 << ", " << corner2 << ", " << corner3 << ": angles=" << sum_angles << endl;
    //cout << "(" << raw_angle(corner2, p, corner1) << ", " << raw_angle(corner3, p, corner2) << ", " << raw_angle(corner1, p, corner3) << endl;
    return (sum_angles == 360.0);
}

bool point_in_triangle(const Triangulation::Triangle_test &triangle, const Triangulation::Point &p) {
    return point_in_triangle(triangle.p1, triangle.p2, triangle.p3, p);
}

vector<Triangulation::Triangle_test> Triangulation::triangulate_ear() {
    vector<Triangle_test> ret;
    unsigned int nb_vertices = polygon.size();
    while (nb_vertices > 3) {
        for (unsigned int i=0; i<nb_vertices; ++i) {
            //cout << "Angle: " << angle(poly[i], poly[(i+1)%nb_vertices], poly[(i+2)%nb_vertices]) << endl;
            if (abs(angle(polygon[i], polygon[(i+1)%nb_vertices], polygon[(i+2)%nb_vertices])) < 180.0) {
                bool no_intersect = true;
                for (const Point &p: polygon) {
                    no_intersect = !point_in_triangle(polygon[i], polygon[(i+1)%nb_vertices], polygon[(i+2)%nb_vertices], p);
                    if (!no_intersect)
                        break;
                }
                if (!no_intersect) {
//                    cout << "Intersect " << (i+1) << endl;
                    continue;
                }
                for (const Triangle_test &triangle: ret) {
                    no_intersect = !point_in_triangle(polygon[i], polygon[(i+1)%nb_vertices], polygon[(i+2)%nb_vertices], triangle.p2);
                    if (!no_intersect)
                        break;
                }
                if (!no_intersect) {
                    //cout << "Intersect " << (i+1) << endl;
                    continue;
                }
//                cout << "adding triangle" << endl;
                ret.push_back({polygon[i], polygon[(i+1)%nb_vertices], polygon[(i+2)%nb_vertices]});
                polygon.erase(polygon.begin() + ((i+1)%nb_vertices));
                break;
            }
        }
        if (polygon.size() != nb_vertices) {
//            cout << "Actual vertices:" << endl << polygon << endl;
//            cout << "Actual triangles:" << endl << ret << endl;
            nb_vertices = polygon.size();
        } else
            break; // No triangle could be found, we stop
    }
    if (nb_vertices == 3)
        ret.push_back({polygon[0], polygon[1], polygon[2]});
    return ret;
}


std::unique_ptr<OGRGeometryCollection> Triangulation::triangulate(TriangulationType type) {
    auto ret = make_unique<OGRGeometryCollection>();
    vector<Triangle_test> v;
    switch (type) {
    case TRIANGULATE_EAR:
        v = triangulate_ear();
        break;
    default: // Do nothing
        break;
    }
    for (const Triangle_test &tt: v) {
        OGRPolygon *p = new OGRPolygon;
        OGRLinearRing *l = new OGRLinearRing;
        l->addPoint(tt.p1.x, tt.p1.y, tt.p1.z);
        l->addPoint(tt.p2.x, tt.p2.y, tt.p2.z);
        l->addPoint(tt.p3.x, tt.p3.y, tt.p3.z);
        p->addRingDirectly(l);
        ret->addGeometryDirectly(p);
    }
    return ret;
}

