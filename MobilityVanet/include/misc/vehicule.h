/**
 * \file	Vehicule.h
 */

#ifndef VEHICULE_H
#define VEHICULE_H

#include <geologic-core/objects/dynamic/MobileObject.h>
#include <geologic-core/objects/geo/untemporal/GeographicalObject.h>
#include <geologic-core/algorithms/geoMetrics/geoMetrics.h>
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
#include "stdio.h"
#include <string>
#include <iostream>
#include <vector>
#include <data/data.h>
#include "ogr_geometry.h"
#include <ogr_spatialref.h>
#include <OpenGL/discdisplayer.h>
#include <OpenGL/multipointsdisplayer.h>
#include <OpenGL/linedisplayer.h>
#include <OpenGL/zonedisplayer.h>
#include <OpenGL/discdisplayer.h>
#include <OpenGL/histogramdisplayer.h>
#include <glrastertile.h>
#include <glvectortile.h>
#include "alerteinfo.h"
#include <unordered_set>

class Fleet;


class Vehicule : public MobileObject, public GeographicalObject, public GeologicGraphicalObject {

    public:
        //*/ -------------------------------------------------
        Vehicule(Fleet* environment);
        virtual ~Vehicule();

        //*/ -------------------------------------------------
        void draw();
        void draw(unsigned long long);

        bool isActive(time_t t);
        void alerteInfo(AlerteInfo alerte, time_t t);
        void mergeColors(Color color);

        //*/ -------------------------------------------------
        Fleet* environment;
        int id;
        std::string statut;
        int heureD;
        int indiceO;
        int indiceD;
        int nbPositions;
        int PosCourante;
        std::vector<int> vPositions;
        Color color;
        bool hasCluster;
        double radius;

        int mergedColors = 0;
        int tabouDuration = 120;
        int transmissionDuration = 20;
        std::vector<std::pair<AlerteInfo, time_t>> setInfo; //message, aquisition time

        void setType();
        std::string getInfos();
};


#endif // VEHICULE_H
