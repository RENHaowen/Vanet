/**
 * \file	Trajectory.h
 */

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <ogr_geometry.h>
#include "geologic-core/graphicTemplates/GeologicGraphicalObjectMobile.h"
#include "geologic-core/objects/transportation/TransportationType.h"
#include "geologic-core/objects/geo/temporal/TemporalGeoPoint.h"
#include "geologic-core/objects/geo/untemporal/GeoZone.h"


class Trajectory : public GeologicGraphicalObjectMobile {

    public:
        //*/ -------------------------------------------------
        Trajectory();
        virtual ~Trajectory();

        Trajectory(Trajectory* trajectory);
        Trajectory(std::vector<TemporalGeoPoint*>* trajectory, TransportationType* transportationType);

        //*/ -------------------------------------------------
        void addPosition(TemporalGeoPoint* position);
        void insertPosition(TemporalGeoPoint* position);
        std::unique_ptr<TemporalGeoPoint> getClosestPoint(time_t timestamp);
        std::unique_ptr<TemporalGeoPoint> getNewPositionAt(time_t timestamp);

        //*/ -------------------------------------------------
        void dummyFill(int slotDuration);
        unsigned int size();
        void clear(bool hard);

        //*/ -------------------------------------------------
        virtual std::unique_ptr<OGRPoint> display(time_t timestamp);
        virtual std::unique_ptr<OGRPoint> display(time_t timestamp, int splineOrder);
        virtual std::unique_ptr<OGRPoint> display(time_t timestamp, std::string approximationAlgorithmName, double factor=0.75);
        virtual std::unique_ptr<OGRPoint> display(time_t timestamp, std::unordered_map<std::string, GeoZone*>* zones);

        //*/ -------------------------------------------------
        std::vector<TemporalGeoPoint*>* trajectory;
        TransportationType* transportationType;

    private:
        //*/ -------------------------------------------------
        bool hasChanged();
        std::unique_ptr<OGRPoint> computeDisplayPoint(time_t timestamp, Trajectory* approximationTrajectory);

        //*/ -------------------------------------------------
        unsigned int lastTrajectorySize;
};

#endif // TRAJECTORY_H
