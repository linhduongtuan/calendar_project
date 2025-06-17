#ifndef PERSIAN_H
#define PERSIAN_H

#include "../../include/calendar_types.h"

// Persian calendar constants
#define PERSIAN_EPOCH_JD 1948321    // Julian day of Persian epoch (March 22, 622 CE)
#define PERSIAN_MONTHS_COUNT 12
#define PERSIAN_DAYS_PER_WEEK 7

// Persian date structure
typedef struct {
    Date base;
    int day_of_week;
    long julian_day;
    int is_leap_year;
    char season[20];  // Spring, Summer, Autumn, Winter
} PersianDate;

// External arrays
extern const char* persian_months[PERSIAN_MONTHS_COUNT];
extern const char* persian_days[PERSIAN_DAYS_PER_WEEK];
extern const int persian_days_per_month[PERSIAN_MONTHS_COUNT];

// Core functions
int persian_is_leap_year(int year);
int persian_days_in_month(int month, int year);
int persian_day_of_week(int day, int month, int year);
long persian_to_julian_day(int day, int month, int year);
CalendarResult persian_validate_date(int day, int month, int year);

// Date creation and manipulation
PersianDate* persian_create_date(int day, int month, int year);
void persian_destroy_date(PersianDate* date);
PersianDate* persian_add_days(const PersianDate* date, int days);
int persian_days_between(const PersianDate* date1, const PersianDate* date2);

// Display functions
void persian_print_month(int month, int year);
void persian_print_year(int year);
void persian_print_date(const PersianDate* date);

// Conversion functions
CalendarResult persian_from_julian_day(long jdn, PersianDate* result);
CalendarResult persian_from_gregorian(const GregorianDate* greg_date, PersianDate* persian_date);
CalendarResult persian_to_gregorian(const PersianDate* persian_date, GregorianDate* greg_date);

// Special Persian calendar functions
double persian_calculate_nowruz(int year);  // Calculate exact Nowruz (New Year) time
const char* persian_get_season(int month);
int persian_get_year_length(int year);  // Returns 365 or 366

#endif // PERSIAN_H