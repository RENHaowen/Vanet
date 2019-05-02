#include "movableview.h"

#include <iostream>
#include <cmath>
#include <cstdio>

#include <Common/color.h>
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>

#include <QtOpenGL/QtOpenGL>

using namespace std;

void MovableView::zoomIn() {
    customToolbox::start("MovableView::zoomIn");
    zoom++;
    if (zoom > 18)
        zoom = 18;
    resize();
    customToolbox::end("MovableView::zoomIn");
}

void MovableView::zoomOut() {
    customToolbox::start("MovableView::zoomOut");
    zoom--;
    if (zoom < 0)
        zoom = 0;
    resize();
    customToolbox::end("MovableView::zoomOut");
}

MovableView::~MovableView() {
    //cout << "MovableView::~MovableView()" << endl;
}

void MovableView::initView() {
    //customToolbox::start("MovableView::initView");
//    if (resized == true) {
        resized = false;
        glViewport (0, 0, (GLsizei) screen_width, (GLsizei) screen_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(view_bbox.minx, view_bbox.maxx, view_bbox.miny, view_bbox.maxy, -200000.0, 200000.0);
        //customToolbox::message("View boundaries: %lf %lf %lf %lf\n", get<0>(view_bbox), get<1>(view_bbox), get<2>(view_bbox), get<3>(view_bbox));
        //cout << "View: " << get<0>(view_bbox) << ", " << get<1>(view_bbox) << ", " << get<2>(view_bbox) << ", " << get<3>(view_bbox) << endl;
        glMatrixMode(GL_MODELVIEW);
//    }

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslated(center_viewref.getX(), center_viewref.getY(), 0.0);
    glRotated(x_rotation, 1.0, 0.0, 0.0);
    glRotated(z_rotation, 0.0, 0.0, 1.0);
    glTranslated(-center_viewref.getX(), -center_viewref.getY(), 0.0);
    //customToolbox::end("MovableView::initView");
}

void MovableView::moveViewCenter(double x_offset, double y_offset) {
    customToolbox::start("MovableView::moveViewCenter(double)");
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    if (geoRefs == nullptr) {
        customToolbox::end("MovableView::moveViewCenter(double)");
        return;
    }
    OGRPoint p{x_offset + center_dataref.getX(), y_offset + center_dataref.getY()};
    p.assignSpatialReference(geoRefs->getDataRef());
    setCenter(p);
    customToolbox::end("MovableView::moveViewCenter(double)");
}

void MovableView::moveViewCenter(int x_offset_px, int y_offset_px) {
    customToolbox::start("MovableView::moveViewCenter(int)");
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    if (geoRefs == nullptr) {
        customToolbox::end("MovableView::moveViewCenter(int)");
        return;
    }
    double x_percent = (double) x_offset_px / (double) screen_width;
    double y_percent = (double) y_offset_px / (double) screen_height;
    double x_offset = x_percent * (data_bbox.maxx - data_bbox.minx);
    double y_offset = y_percent * (data_bbox.maxy - data_bbox.miny);
    moveViewCenter(x_offset, y_offset);
    customToolbox::end("MovableView::moveViewCenter(int)");
}

std::pair<double, double> MovableView::getMinBoundaries() {
    return pair<double, double>{view_bbox.minx, view_bbox.miny};
}

void MovableView::setGeoRefs() {
    customToolbox::start("MovableView::setGeoRefs");
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    if (center_dataref.getSpatialReference() == nullptr) {
        center_dataref.assignSpatialReference(geoRefs->getDataRef());
        //customToolbox::message("No spatial reference.\n");
    } else if (center_dataref.getSpatialReference()->IsSameGeogCS(geoRefs->getDataRef()) == 0) {
        //customToolbox::message("Different spatial reference.\n");
        center_dataref.transformTo(geoRefs->getDataRef());
    }
    center_viewref = center_dataref;
    center_viewref.transformTo(geoRefs->getViewRef());
    resize();
    customToolbox::end("MovableView::setGeoRefs");
}

void MovableView::draw() {
//    Customtoolbox::start("MovableView::draw");
    initView();
    for (const unique_ptr<Layer> &l: vLayer) {
        l->draw();
    }
//    Customtoolbox::end("MovableView::draw");
}

void MovableView::draw(unsigned long long t) {
    customToolbox::start("MovableView::draw(t)");
    initView();
    for (const unique_ptr<Layer> &l: vLayer) {
        l->draw(t);
    }
    customToolbox::end("MovableView::draw(t)");
}

void MovableView::resize() {
    customToolbox::start("MovableView::resize");
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    if (geoRefs == nullptr) {
        customToolbox::end("MovableView::resize [error]");
        return;
    }

    data_bbox.setWGS84Bbox(&center_dataref, screen_width, screen_height, zoom);
    view_bbox.set(data_bbox);
    geoMetrics::convertTo(view_bbox.minx, view_bbox.miny, geoRefs->getDataRef(), geoRefs->getViewRef());
    geoMetrics::convertTo(view_bbox.maxx, view_bbox.maxy, geoRefs->getDataRef(), geoRefs->getViewRef());
    customToolbox::end("MovableView::resize");
}

void MovableView::setCenter(const OGRPoint &p) {
    customToolbox::start("MovableView::setCenter");
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    if (geoRefs == nullptr) {
        customToolbox::end("MovableView::setCenter");
        return;
    }
    center_dataref = p;
    if (center_dataref.getSpatialReference() == nullptr)
        center_dataref.assignSpatialReference(geoRefs->getDataRef());
    if (center_dataref.getSpatialReference()->IsSameGeogCS(geoRefs->getDataRef()) != 0) {
        center_viewref = center_dataref;
        center_viewref.transformTo(geoRefs->getViewRef());
        resize();
    } else {
        cerr << "Cannot change view center: Spatial Reference mismatch." << endl;
    }
    customToolbox::end("MovableView::setCenter");
}


void MovableView::modAngle(double mod_angle) {
    x_rotation += mod_angle;
    if (x_rotation < -75.0) x_rotation = -75.0;
    if (x_rotation > 0.0) x_rotation = 0.0;
}

void MovableView::modZAngle(double mod_angle) {
    z_rotation += mod_angle;
}

void MovableView::registerManager(DataManager* manager) {
    this->manager = manager; //use a vector ?
}

void MovableView::hoverTrackSignal(double posX, double posY) {
    OGRPoint tmp (posX, posY, 0);
    if (manager != nullptr) manager->hoverTrackSignal(&tmp, 0);
}

void MovableView::hoverClickSignal(double posX, double posY) {
    OGRPoint tmp (posX, posY, 0);
    if (manager != nullptr) manager->hoverClickSignal(&tmp, 0);
}

void MovableView::hoverReleaseSignal() {
    if (manager != nullptr) manager->hoverReleaseSignal();
}
