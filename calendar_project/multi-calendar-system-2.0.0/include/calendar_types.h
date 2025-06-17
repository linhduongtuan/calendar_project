#ifndef CALENDAR_TYPES_H
#define CALENDAR_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Common date structure
typedef struct {
    int day;
    int month;
    int year;
} Date;

// Gregorian date structure
typedef struct {
    Date base;
    int day_of_week;  // 0=Sunday, 1=Monday, ..., 6=Saturday
    long julian_day;
} GregorianDate;

// Islamic date structure
typedef struct {
    Date base;
    int is_leap_year;
    char month_name[20];
} IslamicDate;

// Chinese date structure
typedef struct {
    Date base;
    int leap_month;      // 0=regular month, 1=leap month
    char zodiac[20];
    char stem_branch[20];
    int cycle_year;      // Year in 60-year cycle
} ChineseDate;

// Calendar type enumeration
typedef enum {
    CALENDAR_GREGORIAN,
    CALENDAR_ISLAMIC,
    CALENDAR_CHINESE,
    CALENDAR_HEBREW
} CalendarType;

// Function result codes
typedef enum {
    CALENDAR_SUCCESS,
    CALENDAR_ERROR_INVALID_DATE,
    CALENDAR_ERROR_INVALID_MONTH,
    CALENDAR_ERROR_INVALID_YEAR,
    CALENDAR_ERROR_CONVERSION_FAILED,
    CALENDAR_ERROR_MEMORY_ALLOCATION,
    CALENDAR_ERROR_NULL_POINTER
} CalendarResult;

// Constants
#define MAX_DATE_STRING_LENGTH 50
#define MAX_MONTH_NAME_LENGTH 20
#define JULIAN_EPOCH 1721425.5

#endif // CALENDAR_TYPES_H