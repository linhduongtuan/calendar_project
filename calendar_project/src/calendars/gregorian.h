#ifndef GREGORIAN_H
#define GREGORIAN_H

#include "../../include/calendar_types.h"

// Constants
#define GREGORIAN_MONTHS_COUNT 12
#define GREGORIAN_DAYS_PER_WEEK 7

// External arrays
extern const char* gregorian_months[GREGORIAN_MONTHS_COUNT];
extern const char* gregorian_days[GREGORIAN_DAYS_PER_WEEK];
extern const int gregorian_days_per_month[GREGORIAN_MONTHS_COUNT];

// Core functions
int gregorian_is_leap_year(int year);
int gregorian_days_in_month(int month, int year);
int gregorian_day_of_week(int day, int month, int year);
long gregorian_to_julian_day(int day, int month, int year);
CalendarResult gregorian_validate_date(int day, int month, int year);

// Date creation and manipulation
GregorianDate* gregorian_create_date(int day, int month, int year);
void gregorian_destroy_date(GregorianDate* date);
GregorianDate* gregorian_add_days(const GregorianDate* date, int days);
int gregorian_days_between(const GregorianDate* date1, const GregorianDate* date2);

// Display functions
void gregorian_print_month(int month, int year);
void gregorian_print_year(int year);
void gregorian_print_date(const GregorianDate* date);

// Conversion functions
CalendarResult gregorian_from_julian_day(long jdn, GregorianDate* result);

#endif // GREGORIAN_H