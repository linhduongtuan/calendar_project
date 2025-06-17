// src/astronomy/lunar_calc.c (Fixed with proper constants and signatures)
#include "lunar_calc.h"
#include "../calendars/islamic.h"
#include <math.h>

double lunar_calculate_moon_age(long julian_day) {
    // Calculate days since last new moon using precise epoch
    double days_since_new_moon = (julian_day - LUNAR_EPOCH_JD);
    days_since_new_moon = fmod(days_since_new_moon, LUNAR_MONTH_PRECISE);
    if (days_since_new_moon < 0) {
        days_since_new_moon += LUNAR_MONTH_PRECISE;
    }
    return days_since_new_moon;
}

LunarPhase lunar_get_phase(double moon_age) {
    if (moon_age < 1.84566) {
        return LUNAR_NEW_MOON;
    } else if (moon_age < 5.53699) {
        return LUNAR_WAXING_CRESCENT;
    } else if (moon_age < 9.22831) {
        return LUNAR_FIRST_QUARTER;
    } else if (moon_age < 12.91963) {
        return LUNAR_WAXING_GIBBOUS;
    } else if (moon_age < 16.61096) {
        return LUNAR_FULL_MOON;
    } else if (moon_age < 20.30228) {
        return LUNAR_WANING_GIBBOUS;
    } else if (moon_age < 23.99361) {
        return LUNAR_LAST_QUARTER;
    } else {
        return LUNAR_WANING_CRESCENT;
    }
}

const char* lunar_phase_name(LunarPhase phase) {
    switch (phase) {
        case LUNAR_NEW_MOON:        return "New Moon";
        case LUNAR_WAXING_CRESCENT: return "Waxing Crescent";
        case LUNAR_FIRST_QUARTER:   return "First Quarter";
        case LUNAR_WAXING_GIBBOUS:  return "Waxing Gibbous";
        case LUNAR_FULL_MOON:       return "Full Moon";
        case LUNAR_WANING_GIBBOUS:  return "Waning Gibbous";
        case LUNAR_LAST_QUARTER:    return "Last Quarter";
        case LUNAR_WANING_CRESCENT: return "Waning Crescent";
        default:                    return "Unknown";
    }
}

double lunar_calculate_illumination(double moon_age) {
    // Simplified illumination calculation
    double phase_angle = (moon_age / LUNAR_MONTH_PRECISE) * 2.0 * M_PI;
    return (1.0 - cos(phase_angle)) / 2.0;
}

PreciseLunarInfo* lunar_calculate_precise(double julian_day) {
    PreciseLunarInfo* info = malloc(sizeof(PreciseLunarInfo));
    if (!info) return NULL;
    
    info->julian_day = julian_day;
    
    // Julian centuries since J2000.0
    double t = (julian_day - 2451545.0) / 36525.0;
    
    // Moon's mean longitude (degrees)
    double L = 218.3164591 + 481267.88134236 * t - 0.0013268 * t * t + 
               t * t * t / 538841.0 - t * t * t * t / 65194000.0;
    
    // Sun's mean longitude (degrees)
    double L_sun = 280.4664567 + 36000.76982779 * t + 0.0003032 * t * t;
    
    // Moon's mean elongation from sun (degrees)
    double D = L - L_sun;
    D = fmod(D, 360.0);
    if (D < 0) D += 360.0;
    
    // Moon's mean anomaly (degrees)
    double M = 134.9634114 + 477198.8676313 * t + 0.008997 * t * t + 
               t * t * t / 69699.0 - t * t * t * t / 14712000.0;
    M = fmod(M, 360.0);
    
    // Sun's mean anomaly (degrees)
    double M_sun = 357.5291092 + 35999.0502909 * t - 0.0001536 * t * t + 
                   t * t * t / 24490000.0;
    M_sun = fmod(M_sun, 360.0);
    
    // Moon's argument of latitude (degrees)
    double F = 93.2720993 + 483202.0175273 * t - 0.0034029 * t * t - 
               t * t * t / 3526000.0 + t * t * t * t / 863310000.0;
    F = fmod(F, 360.0);
    
    // Convert to radians
    double D_rad = D * M_PI / 180.0;
    double M_rad = M * M_PI / 180.0;
    double M_sun_rad = M_sun * M_PI / 180.0;
    double F_rad = F * M_PI / 180.0;
    
    // Calculate corrections for more precise moon phase
    double correction = 0.0;
    correction += -0.40720 * sin(M_sun_rad);
    correction += 0.17241 * sin(M_rad);
    correction += 0.01608 * sin(2.0 * M_sun_rad);
    correction += 0.01039 * sin(2.0 * F_rad);
    correction += 0.00739 * sin(M_rad - M_sun_rad);
    correction += -0.00514 * sin(M_rad + M_sun_rad);
    correction += 0.00208 * sin(2.0 * M_rad);
    
    D += correction;
    if (D < 0) D += 360.0;
    if (D >= 360.0) D -= 360.0;
    
    info->moon_phase_angle = D;
    info->moon_age_precise = D / 360.0 * LUNAR_MONTH_PRECISE;
    
    // Calculate precise illumination
    info->moon_illumination = (1.0 - cos(D * M_PI / 180.0)) / 2.0;
    
    // Calculate moon distance (simplified Meeus algorithm)
    double lunar_distance_correction = 0.0;
    lunar_distance_correction += -20905.0 * cos(M_rad);
    lunar_distance_correction += -3699.0 * cos(2.0 * D_rad - M_rad);
    lunar_distance_correction += -2956.0 * cos(2.0 * D_rad);
    lunar_distance_correction += -570.0 * cos(2.0 * M_rad);
    
    info->moon_distance_km = 385000.56 + lunar_distance_correction / 1000.0;
    
    // Determine precise phase with better boundaries
    if (D < 11.25 || D >= 348.75) {
        info->phase = LUNAR_NEW_MOON;
    } else if (D < 56.25) {
        info->phase = LUNAR_WAXING_CRESCENT;
    } else if (D < 123.75) {
        info->phase = LUNAR_FIRST_QUARTER;
    } else if (D < 146.25) {
        info->phase = LUNAR_WAXING_GIBBOUS;
    } else if (D < 213.75) {
        info->phase = LUNAR_FULL_MOON;
    } else if (D < 236.25) {
        info->phase = LUNAR_WANING_GIBBOUS;
    } else if (D < 303.75) {
        info->phase = LUNAR_LAST_QUARTER;
    } else {
        info->phase = LUNAR_WANING_CRESCENT;
    }
    
    return info;
}

double lunar_calculate_new_moon_precise(double julian_day) {
    // Use Meeus algorithm for precise new moon calculation
    double k = floor((julian_day - 2451550.09765) / LUNAR_MONTH_PRECISE);
    double T = k / 1236.85;
    
    // Base Julian Day for new moon
    double JDE = 2451550.09765 + LUNAR_MONTH_PRECISE * k
                + 0.0001337 * T * T
                - 0.000000150 * T * T * T
                + 0.00000000073 * T * T * T * T;
    
    // Additional corrections
    double M = 2.5534 + 29.10535669 * k - 0.0000218 * T * T;
    double M_sun = 201.5643 + 385.81693528 * k + 0.0107438 * T * T;
    double F = 160.7108 + 390.67050274 * k - 0.0016341 * T * T;
    double Omega = 124.7746 - 1.56375580 * k + 0.0020691 * T * T;
    
    // Convert to radians
    M *= M_PI / 180.0;
    M_sun *= M_PI / 180.0;
    F *= M_PI / 180.0;
    Omega *= M_PI / 180.0;
    
    // Suppress unused variable warning
    (void)Omega;
    
    // Apply corrections
    double correction = 0.0;
    correction += -0.40720 * sin(M_sun);
    correction += 0.17241 * sin(M);
    correction += 0.01608 * sin(2.0 * M_sun);
    correction += 0.01039 * sin(2.0 * F);
    correction += 0.00739 * sin(M - M_sun);
    correction += -0.00514 * sin(M + M_sun);
    
    return JDE + correction;
}

double lunar_solar_longitude(double julian_day) {
    double t = (julian_day - 2451545.0) / 36525.0;
    
    // Mean longitude of sun
    double L0 = 280.4664567 + 36000.76982779 * t + 0.0003032 * t * t;
    
    // Mean anomaly of sun
    double M = 357.5291092 + 35999.0502909 * t - 0.0001536 * t * t;
    M *= M_PI / 180.0;
    
    // Equation of center
    double C = (1.9146 - 0.004817 * t - 0.000014 * t * t) * sin(M) +
               (0.019993 - 0.000101 * t) * sin(2.0 * M) +
               0.000289 * sin(3.0 * M);
    
    // True longitude
    double longitude = L0 + C;
    longitude = fmod(longitude, 360.0);
    if (longitude < 0) longitude += 360.0;
    
    return longitude;
}

double lunar_moon_longitude(double julian_day) {
    double t = (julian_day - 2451545.0) / 36525.0;
    
    // Moon's mean longitude
    double L = 218.3164591 + 481267.88134236 * t - 0.0013268 * t * t;
    L = fmod(L, 360.0);
    if (L < 0) L += 360.0;
    
    return L;
}

double lunar_equation_of_time(double julian_day) {
    double t = (julian_day - 2451545.0) / 36525.0;
    
    // Simplified equation of time calculation
    double M = 357.5291092 + 35999.0502909 * t;
    M *= M_PI / 180.0;
    
    double equation = 4.0 * (280.4664567 - 357.5291092) + 
                     4.0 * 1.9146 * sin(M);
    
    return equation; // in minutes
}

CalendarResult islamic_from_gregorian_precise(const GregorianDate* greg_date, 
                                             IslamicDate* islamic_date,
                                             double longitude, double latitude) {
    (void)longitude; // For future astronomical calculations
    (void)latitude;  // For future astronomical calculations
    
    // For now, use the existing conversion
    // Future enhancement: Use lunar visibility calculations
    return islamic_from_gregorian(greg_date, islamic_date);
}