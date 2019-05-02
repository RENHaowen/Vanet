/**
 * \file	TemporalGeoData.h
 */

#ifndef TEMPORALGEODATA_H
#define TEMPORALGEODATA_H

#include "geologic-core/graphicTemplates/GeologicGraphicalObjectShape.h"
#include "geologic-core/graphicTemplates/GeologicCustomizeObject.h"
#include <string>
#include <time.h>


class TemporalGeoData : public GeologicGraphicalObjectShape, public GeologicCustomizeObject {

    public:
        //*/ -------------------------------------------------
        TemporalGeoData();
        virtual ~TemporalGeoData();

        //*/ -------------------------------------------------
        bool display(time_t timestamp);
        bool display(time_t timestampOrigin, time_t timestampDestination);

        //*/ -------------------------------------------------
        time_t timestamp;
};

#endif // TEMPORALGEODATA_H
