#ifndef INFODATA_H
#define INFODATA_H
#include <GL/glut.h>
#include <GL/gl.h>
#include <data/data.h>
#include <Common/color.h>
#include <Simulation/simulationclock.h>
#include <sys/time.h>
#include <iostream>

class infoData : public Data {
public:

    infoData();
    ~infoData();

    void draw();
    void draw(unsigned long long t);
};

#endif // INFODATA_H
