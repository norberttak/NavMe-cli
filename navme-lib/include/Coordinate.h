#pragma once
#include "NavMeLibExport.h"

#include "GlobalOptions.h"
#include "Angle.h"
#include "RelativePos.h"

class EXPORT Coordinate {
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

