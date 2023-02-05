#pragma once
#include "NavMeLibExport.h"

#include "GlobalOptions.h"
#include "Airport.h"
#include "RNAVProc.h"
#include "NavPoint.h"

class EXPORT FlightRoute {
private:
    std::string name;
public:
    FlightRoute(std::string _name);
    Airport departure_airport;
    RNAVProc sid;
    std::vector<NavPoint> enroute_points;
    RNAVProc star;
    RNAVProc approach;
    Airport destination_airport;
    Airport alternate_airport;
};