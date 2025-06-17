#ifndef PRAYER_TIMES_H
#define PRAYER_TIMES_H

#include "../../include/calendar_types.h"
#include "../astronomy/solar_calc.h"

// Prayer calculation methods
typedef enum {
    PRAYER_METHOD_MWL,          // Muslim World League
    PRAYER_METHOD_ISNA,         // Islamic Society of North America
    PRAYER_METHOD_EGYPT,        // Egyptian General Authority of Survey
    PRAYER_METHOD_MAKKAH,       // Umm Al-Qura University, Makkah
    PRAYER_METHOD_KARACHI,      // University of Islamic Sciences, Karachi
    PRAYER_METHOD_TEHRAN,       // Institute of Geophysics, University of Tehran
    PRAYER_METHOD_JAFARI,       // Shia Ithna-Ashari, Leva Institute, Qum
    PRAYER_METHOD_CUSTOM        // Custom parameters
} PrayerCalculationMethod;

// Prayer times structure
typedef struct {
    double fajr_jd;             // Dawn prayer
    double sunrise_jd;          // Sunrise (not a prayer, but reference)
    double dhuhr_jd;            // Noon prayer
    double asr_jd;              // Afternoon prayer
    double maghrib_jd;          // Sunset prayer
    double isha_jd;             // Night prayer
    double midnight_jd;         // Islamic midnight
    
    // Additional times
    double tahajjud_jd;         // Night vigil prayer (optional)
    double ishraq_jd;           // Post-sunrise prayer (optional)
    double duha_jd;             // Forenoon prayer (optional)
    
    // Calculation metadata
    PrayerCalculationMethod method;
    double latitude;
    double longitude;
    int timezone_offset;        // hours from UTC
} PrayerTimes;

// Calculation parameters for different methods
typedef struct {
    double fajr_angle;          // Angle below horizon for Fajr
    double isha_angle;          // Angle below horizon for Isha
    double isha_interval;       // Minutes after Maghrib for Isha (if angle not used)
    double maghrib_interval;    // Minutes after sunset for Maghrib
    int asr_method;             // 0=Shafi, 1=Hanafi
    double high_latitude_method; // Method for high latitudes
} PrayerParameters;

// Qibla direction structure
typedef struct {
    double qibla_bearing;       // Degrees from North
    double distance_km;         // Distance to Makkah in kilometers
    double great_circle_bearing; // Great circle bearing
} QiblaDirection;

// Core prayer time calculations
PrayerTimes* prayer_calculate_times(double julian_day, double latitude, double longitude, 
                                   PrayerCalculationMethod method, int timezone_offset);

// Individual prayer calculations
double prayer_calculate_fajr(double julian_day, double latitude, double longitude, double angle);
double prayer_calculate_dhuhr(double julian_day, double longitude);
double prayer_calculate_asr(double julian_day, double latitude, double longitude, int method);
double prayer_calculate_maghrib(double julian_day, double latitude, double longitude);
double prayer_calculate_isha(double julian_day, double latitude, double longitude, double angle);

// Qibla calculations
QiblaDirection* prayer_calculate_qibla(double latitude, double longitude);

// Method parameter getters
PrayerParameters prayer_get_method_parameters(PrayerCalculationMethod method);
void prayer_set_custom_parameters(PrayerParameters* params, double fajr_angle, 
                                 double isha_angle, int asr_method);

// High latitude adjustments
void prayer_adjust_high_latitudes(PrayerTimes* times, double latitude, int method);

// Utility functions
void prayer_print_times(const PrayerTimes* times);
void prayer_print_times_12hour(const PrayerTimes* times);
void prayer_print_qibla(const QiblaDirection* qibla);
const char* prayer_method_name(PrayerCalculationMethod method);

// Memory management
void prayer_destroy_times(PrayerTimes* times);
void prayer_destroy_qibla(QiblaDirection* qibla);

// Time formatting helpers
void prayer_julian_to_time_string(double julian_day, int timezone_offset, char* buffer, int buffer_size);
void prayer_julian_to_12hour_string(double julian_day, int timezone_offset, char* buffer, int buffer_size);

// Validation functions
int prayer_validate_coordinates(double latitude, double longitude);
int prayer_is_valid_method(PrayerCalculationMethod method);

// Special calculations
double prayer_calculate_islamic_midnight(double maghrib_jd, double fajr_jd);
double prayer_calculate_last_third_night(double maghrib_jd, double fajr_jd);
int prayer_is_prayer_time_valid(double prayer_jd, double latitude);

#endif // PRAYER_TIMES_H