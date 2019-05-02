#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T17:42:11
#
#-------------------------------------------------


### ------------------------------------->
QT += network opengl xml core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
CONFIG += c++14
TARGET = geologic-gui
QMAKE_CXXFLAGS += -Wno-comment -Wno-unused-parameter
release: DESTDIR = ./release
release: OBJECTS_DIR = ./obj
### -------------------------------------<

INCLUDEPATH += ../../shapelib
LIBS += -L../../shapelib

### ------------------------------------->
INCLUDEPATH += "$$PWD/include" $$PWD/../geologic-core/include
win32: INCLUDEPATH += c:/msys/1.0/local/include
else:unix: INCLUDEPATH += /usr/include/gdal
### -------------------------------------<


### ------------------------------------->
win32: DEPENDPATH += c:/msys/1.0/local/include $$PWD/../geologic-core/include
### -------------------------------------<


### ------------------------------------->
win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgdal
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgdald
else:unix: LIBS += -lgdal

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -llibGeographic.dll
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -llibGeographic.dlld

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgeos
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgeosd

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lproj
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lprojd

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lshp
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lshpd

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../geologic-core/release/ -lgeologic-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../geologic-core/debug/ -lgeologic-core
else:unix: LIBS += -L$$PWD/../geologic-core/release/ -lgeologic-core

LIBS += -lglut -lm
win32::LIBS += -lopengl32 -lglu32 -lmingw32
### -------------------------------------<


### ------------------------------------->
#geologicgui.cpp \
SOURCES +=    mainwindow.cpp \
    OpenGL/gldisplay.cpp \
    Scene/layer.cpp \
    data/data.cpp \
    Scene/view.cpp \
    Scene/fixedview.cpp \
    Scene/movableview.cpp \
    Simulation/simulationclock.cpp \
    geologicapplication.cpp \
    OpenGL/texture.cpp \
    OpenGL/multipointsdisplayer.cpp \
    OpenGL/linedisplayer.cpp \
    data/staticdata.cpp \
    OpenGL/quaddisplayer.cpp \
    OpenGL/zonedisplayer.cpp \
    OpenGL/histogramdisplayer.cpp \
    Common/config.cpp \
    layercheckbox.cpp \
    preferenceswindow.cpp \
    Common/triangle.cpp \
    Common/triangulation.cpp \
    data/simplegpxparser.cpp \
    OpenGL/discdisplayer.cpp \
    data/datamanager.cpp \
    Scene/singlelayer.cpp \
    Scene/compositelayer.cpp \
    data/clockdata.cpp \
    data/infodata.cpp

HEADERS +=     mainwindow.h \
    OpenGL/gldisplay.h \
    Scene/layer.h \
    data/data.h \
    Scene/view.h \
    Scene/fixedview.h \
    Scene/movableview.h \
    Common/color.h \
    Simulation/simulationclock.h \
    defines.h \
    geologicapplication.h \
    OpenGL/texture.h \
    OpenGL/multipointsdisplayer.h \
    OpenGL/linedisplayer.h \
    data/staticdata.h \
    OpenGL/quaddisplayer.h \
    OpenGL/zonedisplayer.h \
    OpenGL/histogramdisplayer.h \
    Common/config.h \
    layercheckbox.h \
    preferenceswindow.h \
    data/tile.hpp \
    data/tilegrid.hpp \
    Common/triangle.h \
    Common/triangulation.h \
    Common/utils.hpp \
    data/simplegpxparser.h \
    data/mesh.hpp \
    OpenGL/discdisplayer.h \
    data/datamanager.h \
    Scene/singlelayer.h \
    Scene/compositelayer.h \
    data/clockdata.h \
    data/infodata.h
#geologicgui.h\
#geologic-gui_global.h
### -------------------------------------<


### ------------------------------------->
unix {
    target.path = /usr/lib
    INSTALLS += target
}
### -------------------------------------<
