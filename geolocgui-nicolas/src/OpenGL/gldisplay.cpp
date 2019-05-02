#include "OpenGL/gldisplay.h"
#include <QColor>
#include <QWheelEvent>
#include <iostream>
#include <QtOpenGL/QtOpenGL>
#include <geologic-core/utils/customToolbox.h>
#include <exception>
#include <mainwindow.h>
#include <memory.h>
#include <cmath>
#include <Data/testdata.h>
#include <OpenGL/arrow3ddisplayer.h>

#include <ResourceManagement/shadermanager.h>
#include <ResourceManagement/texturemanager.h>
#include <ResourceManagement/texturedownloader.h>
#include <OpenGL/loadingmanager.h>
#include <ResourceManagement/vectortiledatadownloader.h>
#include <ResourceManagement/datamanager.h>

using namespace std;

GLDisplay::GLDisplay(const QGLFormat& format, QWidget *parent): QGLWidget(format, parent) {
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}


GLDisplay::~GLDisplay() {
}


void GLDisplay::initializeGL() {
    ShaderManager::getInstance()->load();
    TextureManager::getInstance()->load();
    DataManager::getInstance()->load();

    gettimeofday(&_last_sec, nullptr);
}


void GLDisplay::paintGL() {
    TextureDownloader::getInstance()->update(); //Try to download a new texture if there is a pending demand
    VectorTileDataDownloader::getInstance()->update(); //Try to download a new vector tile if there is a pending demand
    LoadingManager::getInstance()->update(); //Try to load a Data object into GPU if there is a pending demand
    
    //Update
    if(m_scene_manager) {

        if(m_resized) {
            m_scene_manager->resize(m_window_size);
            m_resized = false;
        }

        double newTime = m_scene_manager->update(); //Next time tick and layers update
        reinterpret_cast<MainWindow *>(this->parent())->getCurrentTimeSlider()->setValue(newTime);
        _time_label->setText(QString::fromStdString(customToolbox::str((int)newTime, true))); //display time as HH:MM:SS
        _time_label->update();
       
        MainWindow *p = (MainWindow *) parent();
        p->setMessage(m_scene_manager->getLabel().c_str());

        m_scene_manager->draw();
    }
    update(); // Shall detect if new data requires screen refresh

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
}


void GLDisplay::resizeGL(int w, int h) {
    m_window_size = QVector2D(w, h);
    m_resized = true;
}


void GLDisplay::wheelEvent(QWheelEvent * event) {
    if (m_scene_manager) {
        m_scene_manager->wheelEvent(event);
    }
}


void GLDisplay::mouseMoveEvent(QMouseEvent * event) {
    if (m_scene_manager) {
        m_scene_manager->mouseMoveEvent(event);
    }
}


void GLDisplay::mousePressEvent(QMouseEvent * event) {
    if (m_scene_manager) {
        m_scene_manager->mousePressEvent(event);
    }
}


void GLDisplay::mouseReleaseEvent(QMouseEvent * event) {
    if (m_scene_manager) {
        m_scene_manager->mouseReleaseEvent(event);
    }
}


void GLDisplay::keyPressEvent(QKeyEvent *event) {
    if (m_scene_manager) {
        m_scene_manager->keyPressEvent(event);
        update();
    }
}


void GLDisplay::keyReleaseEvent(QKeyEvent *event) {
    if (m_scene_manager) {
        m_scene_manager->keyReleaseEvent(event);
        update();
    }
}


void GLDisplay::setGeoRefs() {
}
