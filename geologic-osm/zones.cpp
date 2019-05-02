#include "zones.h"
#include <string>
#include <iostream>
#include <ogr_geometry.h>
#include <algorithm>
#include <array>
#include <utility>
#include <random>
#include <memory>
#include <Common/utils.hpp>
#include <OpenGL/zonedisplayer.h>
#include <OpenGL/linedisplayer.h>

using namespace std;

OGRSpatialReference *Zones::getViewref() const {
    return _viewref;
}

void Zones::setViewref(OGRSpatialReference *viewref) {
    _viewref = viewref;
}

Zones::Zones() {
    geo_ref.importFromEPSG(4326);
    zones_boundaries.assignSpatialReference(&geo_ref);
}

void Zones::load(const char *filename) {
    string filename_std = filename;
    OSMLoader loader;
    loader.loadOSMfile(filename_std);
    osm_node_map_t nodes = loader.getmOSMnode();
    osm_way_map_t ways = loader.getmOSMway();
    osm_relation_map_t relations = loader.getmOSMrelation();

    for (const pair<const long long, osm_relation_t *> &elt: relations) {
        vector<osm_way_t *> border_parts;

        // Get all ways composing the zone border
        for (const osm_member_t &member: elt.second->members) {
            if (member.member_type == READOSM_MEMBER_WAY && member.role == "outer") {
                try {
                    border_parts.push_back(ways.at(member.id));
                } catch (out_of_range &e) {
                    cerr << "Missing zone border part OSM Id [" << member.id << "] " << e.what() << endl;
                }
            }
        }

        if (border_parts.size() == 0) {
            cout << "No border for relation " << elt.first << endl;
            continue;
        }

        // Step 1: copy first border segment, then delete it
        vector<long long> points = border_parts.front()->vnodes_ref;
        long long last_node = points.back();
        border_parts.erase(border_parts.begin());

        // Step 2: while there are borders parts, find the next connected one (having an extremity
        //         point connected to the last added segment), then add it to the array and delete
        //         it from the parts to process.
        while (!border_parts.empty()) {
            auto next_border = find_if(border_parts.begin(), border_parts.end(), [last_node](const osm_way_t *a){
                return a->vnodes_ref.front() == last_node || a->vnodes_ref.back() == last_node;
            });
            if (next_border != border_parts.end()) {
                if (last_node == (*next_border)->vnodes_ref.back()) {
                    for (auto it = (*next_border)->vnodes_ref.rbegin(); it!=(*next_border)->vnodes_ref.rend(); ++it)
                        points.push_back(*it);
                } else {
                    for (auto it = (*next_border)->vnodes_ref.begin(); it!=(*next_border)->vnodes_ref.end(); ++it)
                        points.push_back(*it);
                }
                last_node = points.back();
                border_parts.erase(next_border);
            } else {
                cout << "Incomplete border in relation " << elt.first << " (looking for " << last_node << ")" << endl;
                break;
            }
        }

        // Rebuild complete border
        vector<double> xs;
        vector<double> ys;

        for (long long node_id: points) {
            try {
                xs.push_back(nodes.at(node_id)->longitude);
                ys.push_back(nodes.at(node_id)->latitude);
            } catch (...) {
            }
        }

        // Build the actual border collection
        OGRLineString *border = new OGRLineString;
        border->setPoints(static_cast<int>(xs.size()), xs.data(), ys.data());
        zones_boundaries.addGeometryDirectly(border);
    }
}

void ZonesData::load_from_file(const char *name) {
    _zones->load(name);
    _zones->setViewref(_viewref);
    cout << "Loaded " << _zones->getBoundariesPtr()->getNumGeometries() << " zones" << endl;
}

void ZonesData::draw() {
    if (display_list == 0) {
        display_list = glGenLists(1);
        if (display_list == 0)
            return;
        glNewList(display_list, GL_COMPILE_AND_EXECUTE);
        // Zones background
        Color col{1.0, 1.0, 1.0, 1.0};
        ZoneDisplayer::draw(_zones->getBoundariesPtr(), &col, _viewref);

        // Zones boundaries
        MultiLineDisplayer::draw(_zones->getBoundariesPtr(), &col, 3.0, _viewref);
        glEndList();
    } else {
        glCallList(display_list);
    }
}
