#ifndef LUNAR_BASE_H
#define LUNAR_BASE_H

#include "../../include/calendar_types.h"

// Lunar phase enumeration
typedef enum {
    LUNAR_NEW_MOON,
    LUNAR_WAXING_CRESCENT,
    LUNAR_FIRST_QUARTER,
    LUNAR_WAXING_GIBBOUS,
    LUNAR_FULL_MOON,
    LUNAR_WANING_GIBBOUS,
    LUNAR_LAST_QUARTER,
    LUNAR_WANING_CRESCENT
} LunarPhase;

// Lunar calendar constants (use precise astronomical value)
#define LUNAR_MONTH_DAYS 29.530588531  // Precise synodic month length
#define LUNAR_YEAR_MONTHS 12        // Months in lunar year

// Lunar calendar structure
typedef struct {
    Date base;
    LunarPhase current_phase;
    double moon_age;
    int is_leap_month;
} LunarDate;

// Basic lunar functions
LunarDate* lunar_create_date(int day, int month, int year);
void lunar_destroy_date(LunarDate* date);
void lunar_print_date(const LunarDate* date);

// Moon phase calculations
LunarPhase lunar_calculate_phase(long julian_day);
double lunar_calculate_age(long julian_day);

// Next lunar events (return double for precision)
double lunar_next_new_moon(long julian_day);
double lunar_next_full_moon(long julian_day);
double lunar_previous_new_moon(long julian_day);

// Lunar month utilities
int lunar_days_in_month(int month, int year);
int lunar_is_leap_month(int month, int year);

#endif // LUNAR_BASE_H