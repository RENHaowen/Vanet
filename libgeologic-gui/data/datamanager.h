/**
 * \file	DataManager.h
 */

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <ogr_geometry.h>

class DataManager {

    public:
        //*/ -------------------------------------------------
        DataManager();
        virtual ~DataManager();

        //*/ -------------------------------------------------
        virtual void hoverTrackSignal(OGRPoint* location, time_t t) = 0;
        virtual void hoverClickSignal(OGRPoint* location, time_t t) = 0;
        virtual void hoverReleaseSignal() = 0;

};

#endif // DATAMANAGER_H
