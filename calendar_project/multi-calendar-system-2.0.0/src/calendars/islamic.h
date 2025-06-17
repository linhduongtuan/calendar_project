#ifndef ISLAMIC_H
#define ISLAMIC_H

#include "../../include/calendar_types.h"
#include "lunar_base.h"  // This will include the lunar constants

// Remove the duplicate definition, use the one from lunar_base.h
// #define LUNAR_MONTH_DAYS 29.530589  // Remove this line

// Islamic calendar constants  
#define ISLAMIC_EPOCH_JD 1948439    // Julian day of Islamic epoch (July 16, 622 CE)
#define ISLAMIC_MONTHS_COUNT 12
// External arrays
extern const char* islamic_months[ISLAMIC_MONTHS_COUNT];

// Core functions
int islamic_is_leap_year(int year);
int islamic_days_in_month(int month, int year);
CalendarResult islamic_validate_date(int day, int month, int year);

// Date creation and manipulation
IslamicDate* islamic_create_date(int day, int month, int year);
void islamic_destroy_date(IslamicDate* date);

// Display functions
void islamic_print_month(int month, int year);
void islamic_print_date(const IslamicDate* date);

// Conversion functions
CalendarResult islamic_from_gregorian(const GregorianDate* greg_date, IslamicDate* islamic_date);
CalendarResult islamic_to_gregorian(const IslamicDate* islamic_date, GregorianDate* greg_date);

#endif // ISLAMIC_H