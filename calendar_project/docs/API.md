# API Reference

This document provides comprehensive API documentation for the Multi-Calendar System library functions and interfaces.

## Table of Contents

- [Core API](#core-api)
- [Calendar Systems](#calendar-systems)
- [Astronomical Functions](#astronomical-functions)
- [Utility Functions](#utility-functions)
- [Export Functions](#export-functions)
- [Error Handling](#error-handling)
- [Data Structures](#data-structures)

## Core API

### Calendar Creation and Management

#### `gregorian_create_date()`

Creates a new Gregorian date object with validation.

```c
GregorianDate* gregorian_create_date(int day, int month, int year);
```

**Parameters:**
- `day` - Day of month (1-31)
- `month` - Month (1-12)
- `year` - Year (1-9999)

**Returns:**
- `GregorianDate*` - Pointer to created date object
- `NULL` - If invalid date or memory allocation failed

**Example:**
```c
GregorianDate* date = gregorian_create_date(17, 6, 2025);
if (date) {
    printf("Created: %d/%d/%d\n", date->base.day, date->base.month, date->base.year);
    gregorian_destroy_date(date);
}
```

#### `gregorian_destroy_date()`

Frees memory allocated for a Gregorian date object.

```c
void gregorian_destroy_date(GregorianDate* date);
```

**Parameters:**
- `date` - Pointer to date object to destroy

**Example:**
```c
GregorianDate* date = gregorian_create_date(17, 6, 2025);
// ... use date ...
gregorian_destroy_date(date);
```

### Date Validation

#### `gregorian_validate_date()`

Validates a Gregorian date without creating an object.

```c
CalendarResult gregorian_validate_date(int day, int month, int year);
```

**Parameters:**
- `day` - Day to validate (1-31)
- `month` - Month to validate (1-12)  
- `year` - Year to validate (1-9999)

**Returns:**
- `CALENDAR_SUCCESS` - Date is valid
- `CALENDAR_ERROR_INVALID_DATE` - Date is invalid

**Example:**
```c
if (gregorian_validate_date(29, 2, 2024) == CALENDAR_SUCCESS) {
    printf("Valid leap year date\n");
}
```

### Calendar Conversion

#### `gregorian_to_julian_day()`

Converts Gregorian date to Julian Day Number.

```c
long gregorian_to_julian_day(int day, int month, int year);
```

**Parameters:**
- `day` - Day of month
- `month` - Month (1-12)
- `year` - Year

**Returns:**
- `long` - Julian Day Number

**Example:**
```c
long jdn = gregorian_to_julian_day(17, 6, 2025);
printf("JDN: %ld\n", jdn); // Output: JDN: 2465952
```

#### `julian_day_to_date()`

Converts Julian Day Number to Gregorian date.

```c
void julian_day_to_date(long julian_day, int* day, int* month, int* year);
```

**Parameters:**
- `julian_day` - Julian Day Number
- `day` - Pointer to receive day
- `month` - Pointer to receive month
- `year` - Pointer to receive year

**Example:**
```c
int day, month, year;
julian_day_to_date(2465952, &day, &month, &year);
printf("Date: %d/%d/%d\n", day, month, year); // Output: Date: 17/6/2025
```

### Date Arithmetic

#### `gregorian_add_days()`

Adds days to a Gregorian date.

```c
GregorianDate* gregorian_add_days(const GregorianDate* date, int days);
```

**Parameters:**
- `date` - Source date
- `days` - Number of days to add (can be negative)

**Returns:**
- `GregorianDate*` - New date object with result
- `NULL` - If error occurred

**Example:**
```c
GregorianDate* date = gregorian_create_date(17, 6, 2025);
GregorianDate* future = gregorian_add_days(date, 30);
printf("30 days later: %d/%d/%d\n", 
       future->base.day, future->base.month, future->base.year);

gregorian_destroy_date(date);
gregorian_destroy_date(future);
```

#### `gregorian_days_between()`

Calculates days between two dates.

```c
int gregorian_days_between(const GregorianDate* date1, const GregorianDate* date2);
```

**Parameters:**
- `date1` - Earlier date
- `date2` - Later date

**Returns:**
- `int` - Number of days between dates (positive if date2 > date1)

**Example:**
```c
GregorianDate* start = gregorian_create_date(1, 1, 2025);
GregorianDate* end = gregorian_create_date(17, 6, 2025);
int days = gregorian_days_between(start, end);
printf("Days between: %d\n", days); // Output: Days between: 167
```

## Calendar Systems

### Islamic Calendar

#### `islamic_create_date()`

Creates an Islamic (Hijri) date object.

```c
IslamicDate* islamic_create_date(int day, int month, int year);
```

**Parameters:**
- `day` - Day of month (1-30)
- `month` - Month (1-12)
- `year` - Hijri year

**Returns:**
- `IslamicDate*` - Created Islamic date object
- `NULL` - If invalid or allocation failed

#### `islamic_to_gregorian()`

Converts Islamic date to Gregorian.

```c
CalendarResult islamic_to_gregorian(const IslamicDate* hijri_date, GregorianDate** greg_date);
```

**Parameters:**
- `hijri_date` - Source Islamic date
- `greg_date` - Pointer to receive Gregorian date

**Returns:**
- `CALENDAR_SUCCESS` - Conversion successful
- `CALENDAR_ERROR_INVALID_DATE` - Conversion failed

**Example:**
```c
IslamicDate* hijri = islamic_create_date(1, 1, 1446);
GregorianDate* gregorian;

if (islamic_to_gregorian(hijri, &gregorian) == CALENDAR_SUCCESS) {
    printf("1 Muharram 1446 AH = %d/%d/%d CE\n",
           gregorian->base.day, gregorian->base.month, gregorian->base.year);
}
```

### Chinese Calendar

#### `chinese_create_date()`

Creates a Chinese calendar date object.

```c
ChineseDate* chinese_create_date(int day, int month, int year, bool is_leap_month);
```

**Parameters:**
- `day` - Day of month
- `month` - Lunar month
- `year` - Chinese year
- `is_leap_month` - Whether this is a leap month

#### `chinese_get_zodiac()`

Gets the zodiac animal for a Chinese year.

```c
const char* chinese_get_zodiac(int year);
```

**Parameters:**
- `year` - Chinese calendar year

**Returns:**
- `const char*` - Zodiac animal name

**Example:**
```c
const char* zodiac = chinese_get_zodiac(4722);
printf("Year 4722 is the Year of the %s\n", zodiac); // Year of the Snake
```

### Hebrew Calendar

#### `hebrew_create_date()`

Creates a Hebrew calendar date object.

```c
HebrewDate* hebrew_create_date(int day, int month, int year);
```

**Parameters:**
- `day` - Day of month
- `month` - Hebrew month (1-13, 13 for Adar II in leap years)
- `year` - Hebrew year (Anno Mundi)

#### `hebrew_is_leap_year()`

Determines if a Hebrew year is a leap year.

```c
bool hebrew_is_leap_year(int year);
```

**Parameters:**
- `year` - Hebrew year

**Returns:**
- `true` - Leap year (has Adar II)
- `false` - Regular year

## Astronomical Functions

### Lunar Calculations

#### `calculate_lunar_phase()`

Calculates current lunar phase information.

```c
LunarPhase calculate_lunar_phase(long julian_day);
```

**Parameters:**
- `julian_day` - Julian Day Number for calculation

**Returns:**
- `LunarPhase` - Enum value representing phase

**Lunar Phase Values:**
```c
typedef enum {
    LUNAR_NEW_MOON = 0,
    LUNAR_WAXING_CRESCENT,
    LUNAR_FIRST_QUARTER,
    LUNAR_WAXING_GIBBOUS,
    LUNAR_FULL_MOON,
    LUNAR_WANING_GIBBOUS,
    LUNAR_LAST_QUARTER,
    LUNAR_WANING_CRESCENT
} LunarPhase;
```

#### `calculate_lunar_age()`

Calculates days since last new moon.

```c
double calculate_lunar_age(long julian_day);
```

**Parameters:**
- `julian_day` - Julian Day Number

**Returns:**
- `double` - Days since new moon (0.0 - 29.53)

#### `calculate_lunar_illumination()`

Calculates percentage of moon illuminated.

```c
double calculate_lunar_illumination(long julian_day);
```

**Parameters:**
- `julian_day` - Julian Day Number

**Returns:**
- `double` - Illumination percentage (0.0 - 100.0)

**Example:**
```c
long today_jdn = gregorian_to_julian_day(17, 6, 2025);
LunarPhase phase = calculate_lunar_phase(today_jdn);
double age = calculate_lunar_age(today_jdn);
double illumination = calculate_lunar_illumination(today_jdn);

printf("Lunar Phase: %s\n", lunar_phase_names[phase]);
printf("Age: %.1f days\n", age);
printf("Illumination: %.1f%%\n", illumination);
```

### Solar Calculations

#### `calculate_solar_longitude()`

Calculates the sun's ecliptic longitude.

```c
double calculate_solar_longitude(long julian_day);
```

**Parameters:**
- `julian_day` - Julian Day Number

**Returns:**
- `double` - Solar longitude in degrees (0.0 - 360.0)

## Utility Functions

### String Utilities

#### `format_date_string()`

Formats a date as a string in various formats.

```c
char* format_date_string(const Date* date, DateFormat format);
```

**Parameters:**
- `date` - Date to format
- `format` - Desired format

**Date Format Options:**
```c
typedef enum {
    DATE_FORMAT_ISO,        // 2025-06-17
    DATE_FORMAT_US,         // 06/17/2025
    DATE_FORMAT_EUROPEAN,   // 17/06/2025
    DATE_FORMAT_LONG,       // Tuesday, June 17, 2025
    DATE_FORMAT_SHORT       // Tue, Jun 17, 2025
} DateFormat;
```

#### `parse_date_string()`

Parses a date string into a date object.

```c
CalendarResult parse_date_string(const char* date_str, DateFormat format, Date* result);
```

**Parameters:**
- `date_str` - String to parse
- `format` - Expected format
- `result` - Pointer to receive parsed date

**Returns:**
- `CALENDAR_SUCCESS` - Parsing successful
- `CALENDAR_ERROR_INVALID_DATE` - Invalid format or date

### Day of Week

#### `gregorian_day_of_week()`

Gets day of week for a Gregorian date.

```c
int gregorian_day_of_week(int day, int month, int year);
```

**Parameters:**
- `day` - Day of month
- `month` - Month
- `year` - Year

**Returns:**
- `int` - Day of week (0=Sunday, 1=Monday, ..., 6=Saturday)

#### `day_of_week_name()`

Gets the name of a day of week.

```c
const char* day_of_week_name(int day_of_week);
```

**Parameters:**
- `day_of_week` - Day number (0-6)

**Returns:**
- `const char*` - Day name ("Sunday", "Monday", etc.)

### Leap Year Functions

#### `is_leap_year()`

Generic leap year check for any calendar system.

```c
bool is_leap_year(int year, CalendarType calendar);
```

**Parameters:**
- `year` - Year to check
- `calendar` - Calendar system

**Returns:**
- `true` - Leap year
- `false` - Regular year

## Export Functions

### CSV Export

#### `export_to_csv()`

Exports calendar data to CSV format.

```c
CalendarResult export_to_csv(const char* filename, const ExportData* data);
```

**Parameters:**
- `filename` - Output file path
- `data` - Data to export

**Returns:**
- `CALENDAR_SUCCESS` - Export successful
- `CALENDAR_ERROR_FILE_WRITE` - File write error

### iCal Export

#### `export_to_ical()`

Exports calendar data to iCal format.

```c
CalendarResult export_to_ical(const char* filename, const ExportData* data);
```

**Parameters:**
- `filename` - Output file path  
- `data` - Data to export

**Returns:**
- `CALENDAR_SUCCESS` - Export successful
- `CALENDAR_ERROR_FILE_WRITE` - File write error

## Error Handling

### Error Codes

```c
typedef enum {
    CALENDAR_SUCCESS = 0,
    CALENDAR_ERROR_INVALID_DATE = -1,
    CALENDAR_ERROR_INVALID_MONTH = -2,
    CALENDAR_ERROR_INVALID_YEAR = -3,
    CALENDAR_ERROR_INVALID_DAY = -4,
    CALENDAR_ERROR_OUT_OF_RANGE = -5,
    CALENDAR_ERROR_MEMORY_ALLOCATION = -6,
    CALENDAR_ERROR_FILE_READ = -7,
    CALENDAR_ERROR_FILE_WRITE = -8,
    CALENDAR_ERROR_INVALID_FORMAT = -9,
    CALENDAR_ERROR_INVALID_INPUT = -10
} CalendarResult;
```

### Error Handling Functions

#### `calendar_error_string()`

Gets human-readable error message.

```c
const char* calendar_error_string(CalendarResult error);
```

**Parameters:**
- `error` - Error code

**Returns:**
- `const char*` - Error description

**Example:**
```c
CalendarResult result = gregorian_validate_date(32, 13, 2025);
if (result != CALENDAR_SUCCESS) {
    printf("Error: %s\n", calendar_error_string(result));
}
```

## Data Structures

### Base Date Structure

```c
typedef struct {
    int day;
    int month;
    int year;
} Date;
```

### Gregorian Date

```c
typedef struct {
    Date base;
    long julian_day;
    int day_of_week;
} GregorianDate;
```

### Islamic Date

```c
typedef struct {
    Date base;
    long julian_day;
    char month_name[20];
    bool is_leap_year;
} IslamicDate;
```

### Chinese Date

```c
typedef struct {
    Date base;
    int year_cycle;
    int year_in_cycle;
    bool is_leap_month;
    char zodiac[20];
    char stem_branch[20];
} ChineseDate;
```

### Hebrew Date

```c
typedef struct {
    Date base;
    long julian_day;
    char month_name[20];
    bool is_leap_year;
    int days_in_year;
} HebrewDate;
```

### Lunar Information

```c
typedef struct {
    LunarPhase phase;
    double age;
    double illumination;
    char phase_name[30];
    long next_new_moon_jdn;
    long next_full_moon_jdn;
} LunarInfo;
```

### Export Data

```c
typedef struct {
    Date* dates;
    int count;
    CalendarType source_calendar;
    CalendarType* target_calendars;
    int target_count;
    bool include_lunar_info;
    bool include_holidays;
} ExportData;
```

## Thread Safety

### Thread-Safe Functions

The following functions are thread-safe and can be called concurrently:

- All conversion functions (`*_to_julian_day`, `julian_day_to_*`)
- All validation functions (`*_validate_date`)
- All calculation functions (`calculate_lunar_*`, `gregorian_day_of_week`)
- All utility functions (`format_date_string`, `parse_date_string`)

### Non-Thread-Safe Functions

The following functions are **not** thread-safe:

- Object creation/destruction functions (`*_create_date`, `*_destroy_date`)
- Export functions (`export_to_*`)
- Holiday database functions

For multi-threaded applications, use appropriate synchronization mechanisms.

## Memory Management

### Memory Allocation Guidelines

1. **Object Creation**: Always check return values for NULL
2. **Object Destruction**: Always call corresponding destroy function
3. **String Returns**: Some functions return static strings (don't free)
4. **Dynamic Allocation**: Functions returning allocated memory are documented

### Example Memory Management

```c
// Correct usage
GregorianDate* date = gregorian_create_date(17, 6, 2025);
if (!date) {
    fprintf(stderr, "Failed to create date\n");
    return -1;
}

// Use the date...
gregorian_print_date(date);

// Always clean up
gregorian_destroy_date(date);
```

## Performance Considerations

### Optimization Tips

1. **Cache Julian Day Numbers** for repeated conversions
2. **Validate inputs** before expensive calculations
3. **Reuse date objects** when possible
4. **Batch operations** for multiple dates

### Computational Complexity

| Function | Time Complexity | Space Complexity |
|----------|----------------|------------------|
| `gregorian_to_julian_day` | O(1) | O(1) |
| `julian_day_to_date` | O(1) | O(1) |
| `calculate_lunar_phase` | O(1) | O(1) |
| `gregorian_add_days` | O(1) | O(1) |
| `export_to_csv` | O(n) | O(1) |

---

*API documentation last updated: 2025-06-17 03:16:56 UTC*
*Author: Linh Duong Tuan*