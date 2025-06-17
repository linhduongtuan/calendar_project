// src/calendars/lunar_base.c (Updated with consistent signatures)
#include "lunar_base.h"
#include "../utils/date_utils.h"

LunarDate* lunar_create_date(int day, int month, int year) {
    LunarDate* date = malloc(sizeof(LunarDate));
    if (!date) return NULL;
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    date->moon_age = 0.0;
    date->current_phase = LUNAR_NEW_MOON;
    date->is_leap_month = 0;
    
    return date;
}

void lunar_destroy_date(LunarDate* date) {
    if (date) free(date);
}

void lunar_print_date(const LunarDate* date) {
    if (!date) return;
    printf("Lunar Date: %d/%d/%d (Moon age: %.1f days)\n", 
           date->base.day, date->base.month, date->base.year, date->moon_age);
}

LunarPhase lunar_calculate_phase(long julian_day) {
    double age = lunar_calculate_age(julian_day);
    
    if (age < 1.84566) return LUNAR_NEW_MOON;
    else if (age < 5.53699) return LUNAR_WAXING_CRESCENT;
    else if (age < 9.22831) return LUNAR_FIRST_QUARTER;
    else if (age < 12.91963) return LUNAR_WAXING_GIBBOUS;
    else if (age < 16.61096) return LUNAR_FULL_MOON;
    else if (age < 20.30228) return LUNAR_WANING_GIBBOUS;
    else if (age < 23.99361) return LUNAR_LAST_QUARTER;
    else return LUNAR_WANING_CRESCENT;
}

double lunar_calculate_age(long julian_day) {
    // Simplified lunar age calculation
    double days_since_epoch = julian_day - 2451550.1; // J2000 new moon reference
    return fmod(days_since_epoch, LUNAR_MONTH_DAYS);
}

double lunar_next_new_moon(long julian_day) {
    double age = lunar_calculate_age(julian_day);
    return julian_day + (LUNAR_MONTH_DAYS - age);
}

double lunar_next_full_moon(long julian_day) {
    double age = lunar_calculate_age(julian_day);
    double days_to_full;
    
    if (age < LUNAR_MONTH_DAYS / 2.0) {
        days_to_full = (LUNAR_MONTH_DAYS / 2.0) - age;
    } else {
        days_to_full = LUNAR_MONTH_DAYS - age + (LUNAR_MONTH_DAYS / 2.0);
    }
    
    return julian_day + days_to_full;
}

double lunar_previous_new_moon(long julian_day) {
    double age = lunar_calculate_age(julian_day);
    return julian_day - age;
}

int lunar_days_in_month(int month, int year) {
    (void)month;
    (void)year;
    return 29; // Simplified - alternates between 29 and 30
}

int lunar_is_leap_month(int month, int year) {
    (void)month;
    (void)year;
    return 0; // Simplified implementation
}