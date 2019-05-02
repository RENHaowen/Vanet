#include "osmrouteloader.h"
#include <ogr_spatialref.h>
#include <geologic-core/core/geologic-launcher/CoordinateSystemManager.h>
#include <geologic-core/utils/customToolbox.h>
#include <geologic-core/graphe/Node.h>
#include <geologic-core/graphe/Edge.h>
#include <iostream>
#include <cstdio>

using namespace std;

int OSMRouteLoader::load(std::string &filename) {
    if (loader != nullptr) {
        int ret =  loader->loadOSMfile(filename);
        const osm_node_map_t& mNodes = loader->getmOSMnode();
        const osm_way_map_t& mEdges = loader->getmOSMway();
        map<long long, int> mOccurrences; // Used to count node occurrences in ways
        bool foot, car, cycle; // Flags for road specification
        int oneway; // Three values flag for oneway road specification
        unsigned long long edge_id_max = 0 - 1; // Used to generate new edge ids.

        // Import nodes and build a map for fast access by node id
        for (auto &elt: mNodes) {
            osm_node_t *node = elt.second;
            auto new_node = make_unique<Node>(elt.first, node->longitude, node->latitude);
            new_node->assignSpatialReference(dataref);
            mNode[elt.first] = move(new_node);
        }

        // Go through all ways and their node references vectors to count
        // references to nodes.
        for (auto &elt: mEdges) {
            for (long long ref: elt.second->vnodes_ref) {
                mOccurrences[ref]++;
            }
        }

        for (auto &elt: mEdges) {
            auto new_edge = make_unique<Edge>(elt.first);
            new_edge->getNodes()->assignSpatialReference(dataref);
            string type = edgeType(elt.second);
            foot = isFootEdge(elt.second);
            car = isCarEdge(elt.second);
            cycle = isCycleEdge(elt.second);
            oneway = isOneWay(elt.second);
            for (long long node_id: elt.second->vnodes_ref) {
                new_edge->addNode(mNode[node_id].get());
                // If not first node, not last node and node occurs more than once
                // in edges, then stop this edge, store it and start a new one on
                // current node.
                if (node_id != elt.second->vnodes_ref[0]
                        && node_id != elt.second->vnodes_ref[elt.second->vnodes_ref.size() - 1]
                        && mOccurrences[node_id] > 1) {
                    if (car) new_edge->setCarPermission();
                    if (cycle) new_edge->setCyclePermission();
                    if (foot) new_edge->setFootPermission();
                    if (oneway == 1) new_edge->addOneway(new_edge->last());
                    else if (oneway == -1) new_edge->addOneway(new_edge->first());
                    new_edge->type = type;
                    mEdge[new_edge->id] = move(new_edge); // Stop & store
                    new_edge = make_unique<Edge>(edge_id_max--); // Build a new Edge
                    new_edge->getNodes()->assignSpatialReference(dataref);
                    new_edge->addNode(mNode[node_id].get()); // Start it with node
                }
            }
            if (oneway == 1) new_edge->addOneway(new_edge->last());
            else if (oneway == -1) new_edge->addOneway(new_edge->first());
            if (car) new_edge->setCarPermission();
            if (cycle) new_edge->setCyclePermission();
            if (foot) new_edge->setFootPermission();
            new_edge->type = type;
            mEdge[new_edge->id] = move(new_edge); // Add to vector
        }

        for (auto &e: mEdge) {
            e.second->setDistance();
        }
        return ret;
    }
    return -1;
}

OSMLoader *OSMRouteLoader::getLoader() const {
    return loader.get();
}

void OSMRouteLoader::setLoader(unique_ptr<OSMLoader> &value) {
    loader = move(value);
}

bool OSMRouteLoader::isFootEdge(osm_way_t *w) {
    bool ret = false;
    string t;
    map<string, string>::iterator it = w->tags.find("highway");
    if (it != w->tags.end()) {
        t = it->second;
    }
    if (t == "steps" || t == "primary" || t == "primary_link"
            || t == "secondary" || t == "secondary_link" || t == "tertiary"
            || t == "tertiary_link" || t == "road" || t == "residential"
            || t == "living_street" || t == "footway" || t == "pedestrian"
            || t == "path" || t == "track" || t == "bridleway"
            || t == "unclassified")
        ret = true;
    if ((it = w->tags.find("foot")) != w->tags.end())
        if (it->second == "yes")
            ret = true;

    return ret;
}

bool OSMRouteLoader::isCarEdge(osm_way_t *w) {
    bool ret = false;
    string t;
    map<string, string>::iterator it = w->tags.find("highway");
    if (it != w->tags.end()) {
        t = it->second;
    }
    if (t == "primary" || t == "primary_link" || t == "secondary"
            || t == "secondary_link" || t == "tertiary" || t == "tertiary_link"
            || t == "road" || t == "residential" || t == "living_street"
            || t == "bridleway" || t == "unclassified" || t == "motorway"
            || t == "motorway_link" || t == "trunk" || t == "trunk_link")
        ret = true;

    return ret;
}

bool OSMRouteLoader::isCycleEdge(osm_way_t *w) {
    bool ret = false;
    string t;
    map<string, string>::iterator it = w->tags.find("highway");
    if (it != w->tags.end()) {
        t = it->second;
    }
    if (t == "primary" || t == "primary_link" || t == "secondary"
            || t == "secondary_link" || t == "tertiary" || t == "tertiary_link"
            || t == "road" || t == "residential" || t == "living_street"
            || t == "cycleway" || t == "path" || t == "bridleway"
            || t == "unclassified")
        ret = true;

    return ret;
}

int OSMRouteLoader::isOneWay(osm_way_t *w) {
    int ret = 0;
    map<string, string>::iterator it = w->tags.find("oneway");
    if ((w->tags.find("junction") != w->tags.end())
            && ((w->tags.find("junction"))->second == "roundabout")) {
        ret = 1;
    }
    if (it != w->tags.end()) {
        if (it->second == "yes")
            ret = 1;
        else if (it->second == "-1")
            ret = -1;
    }

    return ret;
}

string OSMRouteLoader::edgeType(osm_way_t *w) {
    string ret = "Unknown";
    map<string, string>::iterator it = w->tags.find("highway");
    if (it != w->tags.end()) {
        ret = it->second;
    }
    return ret;
}
/**/

void OSMRouteLoader::convertGeogCS(OGRSpatialReference *ref)
{
    //for (auto it = vNode.begin(); it != vNode.end(); ++it) {
    for (auto &node: mNode) {
        if (node.second->getSpatialReference() == nullptr)
            node.second->assignSpatialReference(ref);
        else
            node.second->transformTo(ref);
    }
    //for (auto it=vEdge.begin(); it!=vEdge.end(); ++it) {
    for (auto &edge: mEdge) {
        if (edge.second->getNodes()->getSpatialReference() == nullptr)
            edge.second->getNodes()->assignSpatialReference(ref);
        else
            edge.second->getNodes()->transformTo(ref);
    }
}

void OSMRouteLoader::load(const char *filename) {
    string s = filename;
    int i = load(s);
}

void OSMRouteLoader::load(const string &filename) {
    load(filename.c_str());
}
