#ifndef CALENDAR_TYPES_EXTENDED_H
#define CALENDAR_TYPES_EXTENDED_H

#include "calendar_types.h"

// Extended calendar type enumeration
typedef enum {
    CALENDAR_GREGORIAN,
    CALENDAR_ISLAMIC,
    CALENDAR_CHINESE,
    CALENDAR_HEBREW,
    CALENDAR_PERSIAN,           // NEW: Persian/Iranian Solar Hijri
    CALENDAR_BUDDHIST,          // NEW: Buddhist calendar
    CALENDAR_ETHIOPIAN,         // NEW: Ethiopian calendar
    CALENDAR_MAYA,              // NEW: Maya calendar system
    CALENDAR_COPTIC,            // NEW: Coptic calendar
    CALENDAR_JULIAN,            // NEW: Julian calendar
    CALENDAR_INDIAN_NATIONAL,   // NEW: Indian National calendar
    CALENDAR_THAI,              // NEW: Thai solar calendar
    CALENDAR_CUSTOM             // NEW: User-defined calendar
} ExtendedCalendarType;

// Persian date structure (already defined in persian.h)
typedef struct {
    Date base;
    int day_of_week;
    long julian_day;
    int is_leap_year;
    char season[20];
} PersianDate;

// Buddhist date structure
typedef struct {
    Date base;
    int buddhist_era;           // BE year
    char era_name[30];          // Thai, Myanmar, Sri Lankan, etc.
    int lunar_day;              // For lunisolar variants
    char festival[50];          // Associated Buddhist festival
} BuddhistDate;

// Ethiopian date structure
typedef struct {
    Date base;
    int day_of_week;
    long julian_day;
    char month_name_ge[20];     // Ge'ez month name
    char month_name_am[20];     // Amharic month name
    int is_pagume;              // 1 if 13th month (Pagume)
} EthiopianDate;

// Maya date structure
typedef struct {
    Date base;                  // Gregorian correlation
    
    // Long Count
    long long long_count;       // Days since creation
    int baktun;                 // 144,000 days
    int katun;                  // 7,200 days
    int tun;                    // 360 days
    int winal;                  // 20 days
    int kin;                    // 1 day
    
    // Haab (365-day solar year)
    int haab_day;               // 0-19
    char haab_month[20];        // Month name
    
    // Tzolkin (260-day sacred year)
    int tzolkin_number;         // 1-13
    char tzolkin_day[20];       // Day name
    
    // Calendar Round
    int calendar_round_day;     // Position in 52-year cycle
} MayaDate;

// Enhanced lunar phase information
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

// Enhanced astronomical event types
typedef enum {
    ASTRO_EVENT_ECLIPSE_SOLAR,
    ASTRO_EVENT_ECLIPSE_LUNAR,
    ASTRO_EVENT_EQUINOX_SPRING,
    ASTRO_EVENT_EQUINOX_AUTUMN,
    ASTRO_EVENT_SOLSTICE_SUMMER,
    ASTRO_EVENT_SOLSTICE_WINTER,
    ASTRO_EVENT_PLANET_CONJUNCTION,
    ASTRO_EVENT_METEOR_SHOWER,
    ASTRO_EVENT_SUPERMOON,
    ASTRO_EVENT_BLUE_MOON
} AstronomicalEventType;

// Astronomical event structure
typedef struct {
    AstronomicalEventType type;
    double julian_day;
    char name[100];
    char description[200];
    double magnitude;           // For eclipses, brightness, etc.
    double duration_hours;      // Event duration
    int visibility_global;      // 1 if visible globally
    double latitude_center;     // Center of visibility
    double longitude_center;    // Center of visibility
} AstronomicalEvent;

// Holiday categories (extended)
typedef enum {
    HOLIDAY_RELIGIOUS_CHRISTIAN,
    HOLIDAY_RELIGIOUS_ISLAMIC,
    HOLIDAY_RELIGIOUS_JEWISH,
    HOLIDAY_RELIGIOUS_BUDDHIST,
    HOLIDAY_RELIGIOUS_HINDU,
    HOLIDAY_RELIGIOUS_OTHER,
    HOLIDAY_NATIONAL,
    HOLIDAY_CULTURAL,
    HOLIDAY_ASTRONOMICAL,
    HOLIDAY_SEASONAL,
    HOLIDAY_PERSONAL,
    HOLIDAY_CUSTOM
} HolidayCategory;

// Enhanced holiday structure
typedef struct {
    char name[100];
    char description[300];
    ExtendedCalendarType calendar_type;
    HolidayCategory category;
    Date date;
    char country_code[4];
    char region_code[10];       // State/province code
    int is_public_holiday;
    int is_bank_holiday;
    int is_school_holiday;
    double observance_level;    // 0.0-1.0 (how widely observed)
    char traditions[500];       // Traditional observances
} ExtendedHoliday;

// Calendar conversion result with metadata
typedef struct {
    CalendarResult status;
    ExtendedCalendarType source_calendar;
    ExtendedCalendarType target_calendar;
    double conversion_accuracy; // 0.0-1.0
    char notes[200];           // Conversion notes/warnings
} ConversionResult;

// Multi-calendar date structure
typedef struct {
    GregorianDate gregorian;
    IslamicDate islamic;
    ChineseDate chinese;
    PersianDate persian;
    BuddhistDate buddhist;
    EthiopianDate ethiopian;
    MayaDate maya;
    
    long julian_day;           // Common reference point
    AstronomicalEvent* astro_events; // Array of astronomical events
    int astro_event_count;
    ExtendedHoliday* holidays; // Array of holidays for this date
    int holiday_count;
} MultiCalendarDate;

// Calendar system capabilities
typedef struct {
    ExtendedCalendarType type;
    char name[50];
    char description[200];
    int supports_leap_years;
    int supports_lunar_months;
    int supports_astronomical_events;
    int min_year;
    int max_year;
    double accuracy_rating;    // 0.0-1.0
    char regions_used[500];    // Where this calendar is used
} CalendarCapabilities;

// Function pointer types for calendar operations
typedef void* (*CalendarCreateFunc)(int day, int month, int year);
typedef void (*CalendarDestroyFunc)(void* date);
typedef CalendarResult (*CalendarValidateFunc)(int day, int month, int year);
typedef void (*CalendarPrintFunc)(const void* date);
typedef CalendarResult (*CalendarConvertFromJulianFunc)(long jdn, void* result);
typedef long (*CalendarConvertToJulianFunc)(const void* date);

// Calendar system registry entry
typedef struct {
    ExtendedCalendarType type;
    CalendarCapabilities capabilities;
    CalendarCreateFunc create_date;
    CalendarDestroyFunc destroy_date;
    CalendarValidateFunc validate_date;
    CalendarPrintFunc print_date;
    CalendarConvertFromJulianFunc from_julian;
    CalendarConvertToJulianFunc to_julian;
} CalendarSystemEntry;

// Enhanced result codes
typedef enum {
    CALENDAR_SUCCESS,
    CALENDAR_ERROR_INVALID_DATE,
    CALENDAR_ERROR_INVALID_MONTH,
    CALENDAR_ERROR_INVALID_YEAR,
    CALENDAR_ERROR_CONVERSION_FAILED,
    CALENDAR_ERROR_MEMORY_ALLOCATION,
    CALENDAR_ERROR_NULL_POINTER,
    CALENDAR_ERROR_UNSUPPORTED_CALENDAR,
    CALENDAR_ERROR_UNSUPPORTED_OPERATION,
    CALENDAR_ERROR_ASTRONOMICAL_CALCULATION,
    CALENDAR_ERROR_TIMEZONE_INVALID,
    CALENDAR_ERROR_COORDINATE_INVALID,
    CALENDAR_ERROR_FILE_NOT_FOUND,
    CALENDAR_ERROR_PARSE_ERROR,
    CALENDAR_ERROR_NETWORK_ERROR,
    CALENDAR_ERROR_PERMISSION_DENIED
} ExtendedCalendarResult;

// Constants for extended functionality
#define MAX_CALENDAR_SYSTEMS 20
#define MAX_HOLIDAYS_PER_DATE 10
#define MAX_ASTRO_EVENTS_PER_DATE 5
#define MAX_CALENDAR_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 500

#endif // CALENDAR_TYPES_EXTENDED_H