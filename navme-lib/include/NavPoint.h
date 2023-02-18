/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "NavMeLibExport.h"

#include <string>
#include "GlobalOptions.h"
#include "Coordinate.h"

class EXPORT NavPoint {
public:
    typedef enum {
        NONE,
        VOR,
        VOR_DME,
        DME,
        NDB,
        NDB_DME,
        VORTAC,
        RSBN
    } RadioNavType;
protected:
    Angle magnetic_variation;
    Coordinate coordinate;
    std::string icao_region;
    std::string icao_id;
    std::string name;
    RadioNavType radio_type;
    int radio_frequency;
public:
    NavPoint();
    NavPoint(Coordinate _coordinate, std::string _icao_id, std::string _icao_region, Angle _magnetic_variation);
    NavPoint& operator=(const NavPoint& other);
    Coordinate get_coordinate();
    Angle get_magnetic_variation();
    void set_magnetic_variation(Angle _variation);
    void set_coordinate(Coordinate _coord);
    void set_icao_region(std::string _region);
    std::string get_icao_region();
    void set_icao_id(std::string _icao_id);
    std::string get_icao_id();
    void set_name(std::string _name);
    std::string get_name();
    void set_radio_type(RadioNavType _type);
    RadioNavType get_radio_type();
    void set_radio_frequency(int _freq);
    int get_radio_frequency();    
    int planned_altitude;
    int max_altitude;
    int min_altitude;
};

