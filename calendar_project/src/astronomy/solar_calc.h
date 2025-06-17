#ifndef SOLAR_CALC_H
#define SOLAR_CALC_H

#include "../../include/calendar_types.h"
#include <math.h>

// Solar calculation constants
#define SOLAR_EPOCH_2000 2451545.0  // J2000.0 epoch
#define EARTH_ORBITAL_ECCENTRICITY 0.016708634
#define EARTH_AXIAL_TILT 23.43929111  // degrees
#define SOLAR_CONSTANT 1361.0  // W/m²

// Solar position structure
typedef struct {
    double julian_day;
    double solar_longitude;     // degrees
    double solar_latitude;      // degrees (always near 0)
    double right_ascension;     // hours
    double declination;         // degrees
    double equation_of_time;    // minutes
    double solar_distance;      // AU
} SolarPosition;

// Sunrise/sunset calculation structure
typedef struct {
    double sunrise_jd;          // Julian day of sunrise
    double sunset_jd;           // Julian day of sunset
    double solar_noon_jd;       // Julian day of solar noon
    double day_length;          // hours
    double sunrise_azimuth;     // degrees from north
    double sunset_azimuth;      // degrees from north
} SunriseSunset;

// Eclipse information structure
typedef struct {
    double eclipse_jd;          // Julian day of eclipse
    double magnitude;           // Eclipse magnitude (0-1 for partial, >1 for total)
    double duration_minutes;    // Duration in minutes
    double max_eclipse_jd;      // Julian day of maximum eclipse
    int eclipse_type;           // 0=partial, 1=total, 2=annular
} SolarEclipse;

// Seasonal information
typedef struct {
    double spring_equinox_jd;   // Vernal equinox
    double summer_solstice_jd;  // Summer solstice
    double autumn_equinox_jd;   // Autumnal equinox
    double winter_solstice_jd;  // Winter solstice
} SeasonalEvents;

// Core solar calculations
SolarPosition* solar_calculate_position(double julian_day);
double solar_calculate_longitude(double julian_day);
double solar_calculate_declination(double julian_day);
double solar_calculate_equation_of_time(double julian_day);
double solar_calculate_distance(double julian_day);

// Sunrise/sunset calculations
SunriseSunset* solar_calculate_sunrise_sunset(double julian_day, double latitude, double longitude);
double solar_calculate_sunrise(double julian_day, double latitude, double longitude);
double solar_calculate_sunset(double julian_day, double latitude, double longitude);
double solar_calculate_solar_noon(double julian_day, double longitude);

// Seasonal calculations
SeasonalEvents* solar_calculate_seasons(int year);
double solar_calculate_equinox(int year, int is_autumn);
double solar_calculate_solstice(int year, int is_winter);

// Eclipse calculations
SolarEclipse* solar_predict_next_eclipse(double julian_day);
int solar_is_eclipse_visible(double julian_day, double latitude, double longitude);

// Utility functions
double solar_julian_centuries_since_j2000(double julian_day);
double solar_mean_anomaly(double julian_day);
double solar_true_anomaly(double julian_day);
void solar_destroy_position(SolarPosition* pos);
void solar_destroy_sunrise_sunset(SunriseSunset* ss);
void solar_destroy_eclipse(SolarEclipse* eclipse);
void solar_destroy_seasons(SeasonalEvents* seasons);

// Coordinate transformations
void solar_ecliptic_to_equatorial(double longitude, double latitude, double* ra, double* dec);
void solar_equatorial_to_horizontal(double ra, double dec, double latitude, double lst, double* azimuth, double* altitude);

#endif // SOLAR_CALC_H