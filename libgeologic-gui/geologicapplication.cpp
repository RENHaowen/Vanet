#include "geologicapplication.h"

#include <Simulation/simulationclock.h>
#include <Scene/movableview.h>
#include <geologic-core/utils/customToolbox.h>
#include <ogr_geometry.h>
#include <QMenuBar>
#include <QMenu>

#include <iostream>

using namespace std;

MainWindow *GeologicApplication::getMainWindow() const {
    return main_window.get();
}

void GeologicApplication::setMainWindow(MainWindow *value) {
    main_window.reset(value);
    //delete(main_window);
    //main_window = value;
}

GeologicApplication::GeologicApplication(int &argc, char **argv): QApplication(argc, argv) {

    GeologicModule* core = GeologicModule::getInstance();
    //main_window = make_unique<MainWindow>();
    //main_window = new MainWindow();
    //main_window->setWindowTitle(tr("My GIS interface"));

    //*/

    main_window = make_unique<MainWindow>();
    main_window->setWindowTitle(tr("My GIS interface"));

    customToolbox::start("GeologicApplication::GeologicApplication");
    if (argc == 2) {
        cfg.load(argv[1]); // Load configuration file, if provided
    }

    CoordinateSystemManager* geoReferences = CoordinateSystemManager::getInstance();
    geoReferences->setReferences(geoReferences->getCSbyRef(cfg.data_epsg_code).get(),
                                 geoReferences->getCSbyRef(cfg.view_epsg_code).get(),
                                 geoReferences->getCSbyRef(cfg.view_epsg_code).get());

    GLDisplay *glpanel = main_window->getGlPanel();
    if (glpanel != nullptr && geoReferences->getDataRef() != nullptr) {
        glpanel->setGeoRefs();
        OGRPoint view_center{cfg.x, cfg.y};
        view_center.assignSpatialReference(geoReferences->getDataRef());
        main_view = glpanel->getMainView();
        if (main_view != nullptr) {
            main_view->setZoom(cfg.zoom < 0 ? 0 : cfg.zoom > 18 ? 18 : cfg.zoom);
            main_view->setGeoRefs();
            main_view->setCenter(view_center);
        }
    }
    _tools_menu = main_window->menuBar()->addMenu(QObject::tr("Tools"));
    _preferences_window = make_unique<PreferencesWindow>(&cfg);
    _tools_menu->addAction(QObject::tr("Preferences"), _preferences_window.get(), SLOT(show()));

    customToolbox::end("GeologicApplication::GeologicApplication");
    //*/
}

GeologicApplication::~GeologicApplication() {
    //cout << "GeologicApplication::~GeologicApplication()" << endl;
}
