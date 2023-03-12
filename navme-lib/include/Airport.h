/*
 * Copyright 2023 Norbert Takacs
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "GlobalOptions.h"
#include "NavPoint.h"
#include <list>
#include <string>
#include <list>

class Runway {
private:
    const std::string name;
    int course;
    int ils_freq;
    int length;
    int width;
public:
    Runway(std::string _name, int _course, int _ils_freq, int _length, int _width);
    std::string get_name();
    int get_course();
    int get_ils_freq();
    int get_length();
    int get_width();
    void set_course(int _course);
    void set_ils_freq(int _ils_freq);
    void set_length(int _length);
    void set_width(int _width);
};

class Airport : public NavPoint {
private:
    std::list<Runway> runways;
    std::string iata_id;
    std::string city;
    std::string country;
    std::string state;
    int transition_alt;
public:
    Airport(std::string icao_id, std::string _icao_region, Coordinate _coordinate, double _magnetic_deviation);
    Airport();
    Airport& operator=(const Airport& other);
    void add_runway(std::string _name, int _course, int _ils_freq, int _length, int _width);
    Runway* get_runway_by_name(std::string rwy_name);
    std::list<Runway> get_runways();
    std::string get_iata_id();
    void set_iata_id(std::string _iata_id);
    std::string get_city();
    void set_city(std::string _city);
    std::string get_country();
    void set_country(std::string _country);
    std::string get_state();
    void set_state(std::string _state);
    int get_transition_alt();
    void set_transition_alt(int _transition_alt);
    ~Airport();
};