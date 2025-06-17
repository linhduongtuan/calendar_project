// src/astronomy/lunar_calc.h (Updated with proper constants and signatures)
#ifndef LUNAR_CALC_H
#define LUNAR_CALC_H

#include "../../include/calendar_types.h"
#include "../calendars/lunar_base.h"

// Lunar calculation constants
#define LUNAR_EPOCH_JD 1948439.5     // Julian Day of lunar epoch (Jan 1, 622 CE)
#define LUNAR_MONTH_PRECISE 29.530588531  // Precise synodic month in days

// Enhanced lunar phase information
typedef struct {
    double julian_day;
    LunarPhase phase;
    double moon_age_precise;
    double moon_phase_angle;     // Phase angle in degrees
    double moon_illumination;    // Fraction illuminated (0-1)
    double moon_distance_km;     // Distance to moon in kilometers
} PreciseLunarInfo;

// Basic lunar calculations
double lunar_calculate_moon_age(long julian_day);
LunarPhase lunar_get_phase(double moon_age);
const char* lunar_phase_name(LunarPhase phase);
double lunar_calculate_illumination(double moon_age);

// Enhanced astronomical calculations
PreciseLunarInfo* lunar_calculate_precise(double julian_day);
double lunar_calculate_new_moon_precise(double julian_day);
double lunar_solar_longitude(double julian_day);
double lunar_moon_longitude(double julian_day);
double lunar_equation_of_time(double julian_day);

// Enhanced Islamic calendar functions
CalendarResult islamic_from_gregorian_precise(const GregorianDate* greg_date, 
                                             IslamicDate* islamic_date,
                                             double longitude, double latitude);

#endif // LUNAR_CALC_H