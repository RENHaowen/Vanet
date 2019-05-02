#include "Scene/fixedview.h"
#include <iostream>
#include <QtOpenGL/QtOpenGL>
#include <Simulation/simulationclock.h>
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>

using namespace std;

FixedView::FixedView(int vw, int vh) :
        View(), view_width(vw), view_height(vh) {
    //cout << "build fixeview " << endl;

    x_position = 0;

    y_position = 0;
}

FixedView::~FixedView() {
    //cout << "Destroyed FixedView" << endl;
}
/*/
void FixedView::zoomIn() {
}

void FixedView::zoomOut() {
}
/*/
void FixedView::initView() {
    glViewport (x_position, y_position, view_width, view_height);
    //glViewport (0, 0, view_width, view_height);
    //cout << " x position " << x_position << " " << y_position << endl;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    //glOrtho(area_x_min, area_x_max, area_y_min, area_y_max, -1, 1);
    glOrtho(-200.0, 200.0, -200.0, 200.0, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();



}
/*/
void FixedView::updateOrthoBoundaries() {
}

void FixedView::setViewCenter(double x, double y) {
}

void FixedView::moveViewCenter(double x_offset, double y_offset) {
}

std::pair<double, double> FixedView::getMinBoundaries() {
    pair<double, double> mins;
    mins.first = get<0>(view_bbox);
    mins.second = get<1>(view_bbox);
    return mins;
}

void FixedView::modAngle(double mod_angle) {
}
/*/
void FixedView::updateScreenCoordinates() {

    x_position = (screen_width - view_width) / 2;

    y_position = screen_height;
}


void FixedView::draw(unsigned long long t) {

    customToolbox::start("MovableView::draw(t)");

    initView();

    for (const unique_ptr<SingleLayer> &l: vLayer) {

        l->draw(t);
    }

    customToolbox::end("MovableView::draw(t)");

}


void FixedView::draw() {

        //updateScreenCoordinates();

        initView();

        for (const unique_ptr<SingleLayer> &l: vLayer) {

            l->draw();
        }
    //    Customtoolbox::end("MovableView::draw");
}

void FixedView::resize() {
    x_position = (screen_width - view_width) / 2;

    y_position = screen_height - view_height;

    resized = true;
}
