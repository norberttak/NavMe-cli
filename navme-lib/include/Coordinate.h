#pragma once
#include "GlobalOptions.h"
#include "Angle.h"
#include "RelativePos.h"

class Coordinate {
private:
    double earth_radius_km = 6371; //average radius of the Earth in km
    double PI = 3.14159265;
    double calculate_heading_ortho_departure(double lat1, double lng1, double lat2, double lng2);
    double calculate_heading_ortho_arrival(double dep_heading, double lat1, double lng1, double lat2, double lng2);
    double calculate_heading_loxo(double lat1, double lng1, double lat2, double lng2);
public:
    Angle lat; // lateral
    Angle lng; // longitudinal
    double elevation;
    Coordinate();
    Coordinate(double _lat, double _lng, double _elevation);
    Coordinate(Angle _lat, Angle _lng, double _elevation);
    Coordinate(const Coordinate& other);
    Coordinate& operator=(const Coordinate& other);
    void get_relative_pos_to(Coordinate& destination, RelativePos& rel_pos);
    std::string to_string();
};

