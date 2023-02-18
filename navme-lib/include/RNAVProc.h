/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "NavMeLibExport.h"

#include <vector>
#include <string>
#include "GlobalOptions.h"
#include "NavPoint.h"

class EXPORT RNAVProc {
public:
    typedef enum {
        RNAV_SID,
        RNAV_STAR,
        RNAV_APPROACH,
        RNAV_OTHER
    } RNAVProcType;

    RNAVProc(std::string _name, std::string _icao_region, RNAVProcType _type);
    RNAVProc();
    ~RNAVProc();
    RNAVProc& operator=(const RNAVProc& other);
    void add_nav_point(NavPoint nav_pnt);
    std::vector<NavPoint> get_nav_points();
    std::string get_name();
    std::string get_region();
    std::string get_airport_icao_id();
    std::string get_runway_name();
    void set_runway_name(std::string _rwy);
    void set_airport_iaco_id(std::string _airport_icao_id);
    RNAVProcType get_type();
private:
    std::string name;
    std::string icao_region;
    std::string airport_iaco_id;
    std::string rwy;
    RNAVProcType type;
    //std::vector<std::string> nav_point_ids;
    std::vector<NavPoint> nav_points;
};
