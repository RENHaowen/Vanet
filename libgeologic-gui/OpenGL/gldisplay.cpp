#include "OpenGL/gldisplay.h"
#include <QColor>
#include <QWheelEvent>
#include <iostream>
#include <QtOpenGL/QtOpenGL>
#include <geologic-core/utils/customToolbox.h>
#include <exception>
#include <mainwindow.h>
#include <memory.h>
#include <data/clockdata.h>

using namespace std;

GLDisplay::GLDisplay(QWidget *parent): QOpenGLWidget(parent), is_mouse_pressed(false), old_x(0), old_y(0),
    is_running(false), is_paused(true), requires_redraw(false), simu_start(0), simu_end(0) {
    customToolbox::start("GLDisplay::GLDisplay");
    simu_clock = new SimulationClock();
    main_view = make_unique<MovableView>();/**/

    clock_view = make_unique<FixedView>(130,50);
    ClockData *d = new ClockData;
    d->s_clock = simu_clock;
    clock_view->addLayer(make_unique<SingleLayer>(d));

    customToolbox::end("GLDisplay::GLDisplay");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setGeoRefs();
}

GLDisplay::~GLDisplay() {
    delete simu_clock;
    //cout << "GLDisplay::~GLDisplay()" << endl;
}

void GLDisplay::initializeGL()
{
    customToolbox::start("GLDisplay::initializeGL");
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_FASTEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_FASTEST);

    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH, GL_FASTEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    requires_redraw = true;
    customToolbox::end("GLDisplay::initializeGL");
    gettimeofday(&_last_sec, nullptr);
}

void GLDisplay::paintGL()
{
    //    Customtoolbox::start("GLDisplay::paintGL");

    if (is_running && simu_clock != nullptr) {
        simu_clock->update();
        unsigned long long t = simu_clock->getCurrentTime();
        if (t > simu_end) {
            is_running = false;
            simu_clock->stop();
            t = simu_end;
        }

        _time_label->setText(QString::fromStdString(customToolbox::str(t, true))); //display time as HH:MM:SS
        _time_label->update();

        reinterpret_cast<MainWindow *>(this->parent())->getCurrentTimeSlider()->setValue(t);
        draw(t);
        update();
    } else {
        draw();
        update(); // Shall detect if new data requires screen refresh
    }/**/
    // FPS calculation
    ++_images;
    struct timeval current_time;
    gettimeofday(&current_time, nullptr);
    if (current_time.tv_sec > _last_sec.tv_sec) {
        double fps = static_cast<double>(_images) / (current_time.tv_sec - _last_sec.tv_sec);
        _fps_label->setNum(fps);
        _fps_label->update();
        _images = 0;
        _last_sec.tv_sec = current_time.tv_sec;
    }
    //    Customtoolbox::end("GLDisplay::paintGL");
}

void GLDisplay::resizeGL(int w, int h)
{
    customToolbox::start("GLDisplay::resizeGL");

    main_view->setWindowSize(w, h);
    clock_view->setWindowSize(w,h);
    update();

    customToolbox::end("GLDisplay::resizeGL");
}

void GLDisplay::draw()
{
    //    Customtoolbox::start("GLDisplay::draw");
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_view->draw();
    clock_view->draw();
    //glFinish();
    //swapBuffers();
    //    Customtoolbox::end("GLDisplay::draw");
}

void GLDisplay::draw(unsigned long long t)
{
    customToolbox::start("GLDisplay::draw(t)");
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_view->draw(t);
    clock_view->draw(t);
    //glFinish();
    //swapBuffers();
    customToolbox::end("GLDisplay::draw(t)");
}

void GLDisplay::wheelEvent(QWheelEvent * event) {
    customToolbox::start("GLDisplay::wheelEvent");
    if (event != nullptr) {
        //int numDegrees = event->delta() / 8;
        //cout << "Degree: " << numDegrees << endl;
        //int numSteps = numDegrees / 15;
        if (event->delta() > 0)
            main_view->zoomIn();
        else
            main_view->zoomOut();
        update();
        event->accept();
    }
    customToolbox::end("GLDisplay::wheelEvent");
}

void GLDisplay::mouseMoveEvent(QMouseEvent * event) {
    customToolbox::start("GLDisplay::mouseMoveEvent");
    if (is_mouse_pressed && event != nullptr) {
        int xpx_offset = old_x - event->pos().x();
        int ypx_offset = event->pos().y() - old_y;
        hasMovedBtwClick = true;
        main_view->moveViewCenter(xpx_offset, ypx_offset);
        update();
    }/**/
    if (is_alt_pressed) {
        // Change view X angle according to mouse movement
        double a = (double) (old_y - event->pos().y()) / (double) height() * 200.0;
        main_view->modAngle(a);
        // Change view Y angle according to mouse movement
        a = (double) (event->pos().x() - old_x) / (double) width() * 200.0;
        main_view->modZAngle(a);
        update();
    }
    MainWindow *p = (MainWindow *) parent();
    double x = main_view->getViewXMin() + ((main_view->getViewXMax() - main_view->getViewXMin()) * event->pos().x() / width());
    double y = main_view->getViewYMin() + ((main_view->getViewYMax() - main_view->getViewYMin()) * (height() - event->pos().y()) / height());
    char message[100];

    sprintf(message, "%lf, %lf, %s", x, y, displayRefLabel.c_str());
    p->setMessage(message);
    old_x = event->pos().x();
    old_y = event->pos().y();

    if (is_ctrl_pressed) main_view->hoverTrackSignal(x, y);

    customToolbox::end("GLDisplay::mouseMoveEvent");
}

void GLDisplay::mousePressEvent(QMouseEvent * event) {
    customToolbox::start("GLDisplay::mousePressEvent");
    if (event != nullptr && event->button() == Qt::LeftButton) {
        is_mouse_pressed = true;
        old_x = event->pos().x();
        old_y = event->pos().y();
    }
    hasMovedBtwClick = false;
    customToolbox::end("GLDisplay::mousePressEvent");
}

void GLDisplay::mouseReleaseEvent(QMouseEvent * event) {
    customToolbox::start("GLDisplay::mouseReleaseEvent");
    if (event != nullptr && event->button() == Qt::LeftButton) {
        is_mouse_pressed = false;
    }
    if (is_ctrl_pressed && !hasMovedBtwClick) {
        double x = main_view->getViewXMin() + ((main_view->getViewXMax() - main_view->getViewXMin()) * event->pos().x() / width());
        double y = main_view->getViewYMin() + ((main_view->getViewYMax() - main_view->getViewYMin()) * (height() - event->pos().y()) / height());
        main_view->hoverClickSignal(x, y);
    }
    customToolbox::end("GLDisplay::mouseReleaseEvent");
}

void GLDisplay::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) {
        main_view->reinitializeAngles();
        update();
    } else if (event->key() == Qt::Key_Alt) {
        is_alt_pressed = true;
        setMouseTracking(true);
    } else if (event->key() == Qt::Key_Control) {
        is_ctrl_pressed = true;
        setMouseTracking(true);
    }
}

void GLDisplay::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Alt) {
        is_alt_pressed = false;
        setMouseTracking(true);
    } else if (event->key() == Qt::Key_Control) {
        is_ctrl_pressed = false;
        setMouseTracking(true);
        main_view->hoverReleaseSignal();
    }
}/**/

void GLDisplay::start() {
    customToolbox::start("GLDisplay::start");
    if (simu_start < simu_end && simu_clock != nullptr) {
        if (!is_running) {
            is_running = true;
            simu_clock->setInitialTime(simu_start);
        }
        if (is_paused) is_paused = false;
        simu_clock->start();
        update();
    }
    customToolbox::end("GLDisplay::start");
}

void GLDisplay::pause() {
    customToolbox::start("GLDisplay::pause");
    if (simu_clock != nullptr)
        if (!is_paused) is_paused = true;
    simu_clock->pause();
    update();
    customToolbox::end("GLDisplay::pause");
}

void GLDisplay::stop() {
    customToolbox::start("GLDisplay::stop");
    is_running = false;
    if (simu_clock != nullptr)
        simu_clock->stop();
    customToolbox::end("GLDisplay::stop");
}

void GLDisplay::setSimulationTimeScale(double s) {
    customToolbox::start("GLDisplay::setSimulationTimeScale");
    //if (s >= MIN_TIME_SCALE && s <= MAX_TIME_SCALE) {
    if (simu_clock != nullptr)
        simu_clock->setTimeScale(s, !is_paused);
    //}
    customToolbox::end("GLDisplay::setSimulationTimeScale");
}

void GLDisplay::setSimulationStartTime(unsigned long long d) {
    customToolbox::start("GLDisplay::setSimulationStartTime");
    simu_start = d;
    //progressBar->SetMin(d);
    customToolbox::end("GLDisplay::setSimulationStartTime");
}

void GLDisplay::setSimulationEndTime(unsigned long long d) {
    customToolbox::start("GLDisplay::setSimulationEndTime");
    simu_end = d;
    customToolbox::end("GLDisplay::setSimulationEndTime");
}

void GLDisplay::setSimulationCurrentTime(unsigned long long d) {
    customToolbox::start("GLDisplay::setSimulationCurrentTime");
    if (simu_clock != nullptr)
        simu_clock->setCurrentTime(d);
    customToolbox::end("GLDisplay::setSimulationCurrentTime");
}

void GLDisplay::setGeoRefs() {
    CoordinateSystemManager* geoRefs = CoordinateSystemManager::getInstance();
    displayRef = geoRefs->getDisplayRef();
    displayRefLabel = geoRefs->getCSName(displayRef) + " (EPSG:" + customToolbox::str(displayRef->GetEPSGGeogCS()) + ")";
}
