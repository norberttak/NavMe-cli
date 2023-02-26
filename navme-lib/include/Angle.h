#pragma once
#include "GlobalOptions.h"
#include <string>

class Angle {
private:
    void convert_from_double(double angle);
    bool sign_negative = false;
    int degree;
    int minute;
    int second;
public:
    Angle();
    Angle(int _degree, int _minute, int _second);
    Angle(double angle);
    Angle(int _degree, double minute_dec);
    double convert_to_double();
    double convert_to_radian();
    std::string to_string(bool use_abs);
    Angle& operator=(const Angle& other);
    Angle& operator=(const double& d_val);
    bool operator==(const Angle& other);
    bool operator!=(const Angle& other);
    Angle operator+(Angle& other);
    Angle operator-(Angle& other);
    int get_degree();
    int get_minute();
    int get_second();
};

