/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "NavMeLibExport.h"
#include "GlobalOptions.h"
#include "NavPoint.h"
#include <list>
#include <string>
#include <list>

class EXPORT Runway {
private:
    const std::string name;
    const int course;
    const int ils_freq;
    const int length;
public:
    Runway(std::string _name, int _course, int _ils_freq, int _length);
    std::string get_name();
    int get_course();
    int get_ils_freq();
    int get_length();
};

class EXPORT Airport : public NavPoint {
private:
    std::list<Runway> runways;
public:
    Airport(std::string icao_id, std::string _icao_region, Coordinate _coordinate, double _magnetic_deviation);
    Airport();
    Airport& operator=(const Airport& other);
    void add_runway(std::string _name, int _course, int _ils_freq, int _length);
    std::list<Runway> get_runways();
    ~Airport();
};