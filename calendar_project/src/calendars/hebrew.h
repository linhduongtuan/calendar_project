// src/calendars/hebrew.h (Updated with enhanced functions)
#ifndef HEBREW_H
#define HEBREW_H

#include "../../include/calendar_types.h"

// Hebrew calendar constants
#define HEBREW_EPOCH_JD 347998.5  // More precise Julian day of Hebrew epoch
#define HEBREW_MONTHS_COUNT 13    // 12 regular + 1 leap month
#define MOLAD_SECONDS (29.5305888531 * 24 * 3600)  // Precise lunation in seconds

// Hebrew date structure
typedef struct {
    Date base;
    int is_leap_year;
    int leap_month_added;  // Which month is duplicated in leap year
    char month_name[30];
    char hebrew_date[50];  // Hebrew text representation
} HebrewDate;

// Hebrew month names
extern const char* hebrew_months[HEBREW_MONTHS_COUNT];
extern const char* hebrew_months_hebrew[HEBREW_MONTHS_COUNT];

// Core functions
int hebrew_is_leap_year(int year);
int hebrew_months_in_year(int year);
int hebrew_days_in_month(int month, int year);
long hebrew_elapsed_days(int year);
long hebrew_elapsed_days_accurate(int year);  // Enhanced version

// Date creation and manipulation
HebrewDate* hebrew_create_date(int day, int month, int year);
void hebrew_destroy_date(HebrewDate* date);

// Conversion functions
CalendarResult hebrew_from_gregorian(const GregorianDate* greg_date, HebrewDate* hebrew_date);
CalendarResult hebrew_from_gregorian_enhanced(const GregorianDate* greg_date, HebrewDate* hebrew_date);
CalendarResult hebrew_to_gregorian(const HebrewDate* hebrew_date, GregorianDate* greg_date);

// Display functions
void hebrew_print_date(const HebrewDate* date);
void hebrew_print_month(int month, int year);

#endif // HEBREW_H