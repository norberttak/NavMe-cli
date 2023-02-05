#pragma once
#include <string>

#include "Angle.h"

struct RelativePos {
    Angle heading_ortho_departure;// departure heading orthodromic (great circle)
    Angle heading_ortho_arrival;// arrival heading orthodromic (great circle)
    double dist_ortho;// distance orthodrom

    Angle heading_loxo;// heading loxodromic
    double dist_loxo;// distance loxodrom
};