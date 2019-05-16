/**
 * \file	GeologicCluster.cpp
 * \class	GeologicCluster
 * \author	Etienne Thuillier (etienne.thuillier@utbm.fr)
 * \date	10/04/2016
 * \brief	Methods of GeologicCluster object
 *
 */

#include "geologic-core/algorithms/clustering/GeologicCluster.h"
#include "geologic-core/algorithms/clustering/ClusteringEnvironment.h"


using namespace std;
using namespace customToolbox;
using namespace geoMetrics;


/**
 * \brief 	Default constructor of class ClusterKmeans
 *
 * \param 	name    The name of the cluster
 * \return 	None
 *
 * Centroid and timestamp are set to nullptr
 */
GeologicCluster::GeologicCluster(string id, ClusteringEnvironment* env) {
    this->id = id;
    this->env = env;
    this->name = id;
}

/**
 * \brief 	Default destructor of class ClusterKmeans
 *
 * \param 	name    The name of the cluster
 * \return 	None
 *
 * Centroid and timestamp are set to nullptr
 */
GeologicCluster::~GeologicCluster() {
    this->records.clear();
    this->env = nullptr;
}

/**
 * \brief 	Default constructor of class ClusterKmeans
 *
 * \param 	name    The name of the cluster
 * \return 	None
 *
 * Centroid and timestamp are set to nullptr
 */
void GeologicCluster::addObject(GeographicalObject* object) {
    if (!isIn(this->records, object)) {
        this->records.push_back(object);
    }
}

/**
 * \brief 	Default constructor of class ClusterKmeans
 *
 * \param 	name    The name of the cluster
 * \return 	None
 *
 * Centroid and timestamp are set to nullptr
 */
bool GeologicCluster::computeCentroid() {
   // cout << "false " << endl;
    //cout << this->id << endl;

    if (this->records.size() <= 0) {

        //cout << "<=  " << endl;

        setX(0);
        setY(0);
        //cout << "false " << endl;
        return false;
    }

    double newX = 0;
    double newY = 0;
    for (unsigned int i=0; i<this->records.size(); i++) {

        //cout << ">  " << endl;

        newX = ((newX*i) + this->records[i]->getX())/(i+1.0);
        newY = ((newY*i) + this->records[i]->getY())/(i+1.0);
    }
    setX(newX);
    setY(newY);
    //cout << "centrre " << newX << " " << newY << endl;

    //cout << "compute fin " << endl;
    return true;
}

/**
 * \brief 	Default constructor of class ClusterKmeans
 *
 * \param 	name    The name of the cluster
 * \return 	None
 *
 * Centroid and timestamp are set to nullptr
 */
void GeologicCluster::clearRecords() {
    this->records.clear(); //WHy do we keep this method ?
}




