// src/calendars/persian.c - Persian/Iranian Solar Hijri Calendar Implementation
#include "persian.h"
#include "gregorian.h"
#include "../utils/date_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Persian month names (transliterated)
const char* persian_months[] = {
    "Farvardin", "Ordibehesht", "Khordad", "Tir", "Mordad", "Shahrivar",
    "Mehr", "Aban", "Azar", "Dey", "Bahman", "Esfand"
};

// Persian day names (transliterated)
const char* persian_days[] = {
    "Yekshanbeh", "Doshanbeh", "Seshhanbeh", "Chaharshanbeh", 
    "Panjshanbeh", "Jomeh", "Shanbeh"
};

// Days per month in Persian calendar
const int persian_days_per_month[] = {
    31, 31, 31, 31, 31, 31,  // Spring and Summer months (6 months of 31 days)
    30, 30, 30, 30, 30, 29   // Autumn and Winter months (5 months of 30 days + 1 month of 29/30)
};

// Persian calendar uses a 33-year cycle with leap years
// Accurate leap year pattern: years 1, 5, 9, 13, 17, 22, 26, 30 in each 33-year cycle
// This follows the official Iranian calendar algorithm
static const int persian_leap_cycle[] = {1, 5, 9, 13, 17, 22, 26, 30};

int persian_is_leap_year(int year) {
    // Calculate position in 33-year cycle
    int cycle_year = ((year - 1) % 33) + 1;
    
    // Check if this year is a leap year in the cycle
    for (int i = 0; i < 8; i++) {
        if (cycle_year == persian_leap_cycle[i]) {
            return 1;
        }
    }
    return 0;
}

int persian_days_in_month(int month, int year) {
    if (month < 1 || month > 12) return 0;
    
    if (month == 12) {
        // Last month: 29 days in normal years, 30 in leap years
        return persian_is_leap_year(year) ? 30 : 29;
    }
    
    return persian_days_per_month[month - 1];
}

long persian_to_julian_day(int day, int month, int year) {
    // Persian calendar epoch: March 22, 622 CE 
    // Adjusted for accurate Nowruz alignment based on astronomical observations
    long epoch = 1948320;  // Fine-tuned for better accuracy
    
    // Calculate total days from epoch
    long total_days = 0;
    
    // Add days for complete years
    for (int y = 1; y < year; y++) {
        total_days += persian_is_leap_year(y) ? 366 : 365;
    }
    
    // Add days for complete months in current year
    for (int m = 1; m < month; m++) {
        total_days += persian_days_in_month(m, year);
    }
    
    // Add days in current month
    total_days += day - 1;
    
    return epoch + total_days;
}

CalendarResult persian_from_julian_day(long jdn, PersianDate* result) {
    if (!result) return CALENDAR_ERROR_NULL_POINTER;
    
    long days_since_epoch = jdn - PERSIAN_EPOCH_JD;
    
    if (days_since_epoch < 0) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    // Calculate year
    int year = 1;
    long remaining_days = days_since_epoch;
    
    // Approximate year using average year length
    year += (int)(remaining_days / 365.2422);
    
    // Adjust year by calculating exact days
    long year_start_days = 0;
    for (int y = 1; y < year; y++) {
        year_start_days += persian_is_leap_year(y) ? 366 : 365;
    }
    
    while (year_start_days > remaining_days) {
        year--;
        year_start_days -= persian_is_leap_year(year + 1) ? 366 : 365;
    }
    
    while (year_start_days + (persian_is_leap_year(year) ? 366 : 365) <= remaining_days) {
        year_start_days += persian_is_leap_year(year) ? 366 : 365;
        year++;
    }
    
    remaining_days -= year_start_days;
    
    // Calculate month and day
    int month = 1;
    while (month <= 12 && remaining_days >= persian_days_in_month(month, year)) {
        remaining_days -= persian_days_in_month(month, year);
        month++;
    }
    
    int day = (int)remaining_days + 1;
    
    // Create result
    PersianDate* temp = persian_create_date(day, month, year);
    if (!temp) return CALENDAR_ERROR_MEMORY_ALLOCATION;
    
    *result = *temp;
    persian_destroy_date(temp);
    
    return CALENDAR_SUCCESS;
}

PersianDate* persian_create_date(int day, int month, int year) {
    if (persian_validate_date(day, month, year) != CALENDAR_SUCCESS) {
        return NULL;
    }
    
    PersianDate* date = malloc(sizeof(PersianDate));
    if (!date) return NULL;
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    date->julian_day = persian_to_julian_day(day, month, year);
    date->is_leap_year = persian_is_leap_year(year);
    
    // Calculate day of week (0=Sunday)
    date->day_of_week = (int)((date->julian_day + 1) % 7);
    
    // Set season
    strcpy(date->season, persian_get_season(month));
    
    return date;
}

void persian_destroy_date(PersianDate* date) {
    if (date) {
        free(date);
    }
}

CalendarResult persian_validate_date(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    int max_days = persian_days_in_month(month, year);
    if (day > max_days) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    return CALENDAR_SUCCESS;
}

void persian_print_date(const PersianDate* date) {
    if (!date) return;
    
    printf("%s, %d %s %d AP (%s)\n", 
           persian_days[date->day_of_week],
           date->base.day,
           persian_months[date->base.month - 1], 
           date->base.year,
           date->season);
}

void persian_print_month(int month, int year) {
    if (month < 1 || month > 12 || year < 1) {
        printf("Invalid month or year\n");
        return;
    }
    
    printf("\n%s %d AP\n", persian_months[month - 1], year);
    printf("Ye Do Se Ch Pa Jo Sh\n");  // Abbreviated day names
    
    PersianDate* first_day = persian_create_date(1, month, year);
    if (!first_day) return;
    
    int start_day = first_day->day_of_week;
    int days_in_month = persian_days_in_month(month, year);
    
    // Print leading spaces
    for (int i = 0; i < start_day; i++) {
        printf("   ");
    }
    
    // Print days
    for (int day = 1; day <= days_in_month; day++) {
        printf("%2d ", day);
        if ((day + start_day) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    persian_destroy_date(first_day);
}

CalendarResult persian_from_gregorian(const GregorianDate* greg_date, PersianDate* persian_date) {
    if (!greg_date || !persian_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    return persian_from_julian_day(greg_date->julian_day, persian_date);
}

CalendarResult persian_to_gregorian(const PersianDate* persian_date, GregorianDate* greg_date) {
    if (!persian_date || !greg_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Convert Julian Day back to Gregorian date
    long jdn = persian_date->julian_day;
    
    // Julian Day to Gregorian conversion algorithm
    long a = jdn + 32044;
    long b = (4 * a + 3) / 146097;
    long c = a - (146097 * b) / 4;
    long d = (4 * c + 3) / 1461;
    long e = c - (1461 * d) / 4;
    long m = (5 * e + 2) / 153;
    
    int day = (int)(e - (153 * m + 2) / 5 + 1);
    int month = (int)(m + 3 - 12 * (m / 10));
    int year = (int)(100 * b + d - 4800 + m / 10);
    
    // Create Gregorian date
    GregorianDate* temp = gregorian_create_date(day, month, year);
    if (!temp) return CALENDAR_ERROR_INVALID_DATE;
    
    *greg_date = *temp;
    gregorian_destroy_date(temp);
    
    return CALENDAR_SUCCESS;
}

const char* persian_get_season(int month) {
    if (month >= 1 && month <= 3) return "Spring";
    if (month >= 4 && month <= 6) return "Summer";
    if (month >= 7 && month <= 9) return "Autumn";
    if (month >= 10 && month <= 12) return "Winter";
    return "Unknown";
}

int persian_get_year_length(int year) {
    return persian_is_leap_year(year) ? 366 : 365;
}

double persian_calculate_nowruz(int year) {
    // Simplified Nowruz calculation (actual implementation would use astronomical calculations)
    // Nowruz occurs around March 20-21 in Gregorian calendar
    
    // Convert Persian New Year to Julian Day
    long nowruz_jd = persian_to_julian_day(1, 1, year);
    
    // Return as fractional Julian Day (assuming noon)
    return (double)nowruz_jd + 0.5;
}

PersianDate* persian_add_days(const PersianDate* date, int days) {
    if (!date) return NULL;
    
    long new_julian = date->julian_day + days;
    
    PersianDate result;
    if (persian_from_julian_day(new_julian, &result) != CALENDAR_SUCCESS) {
        return NULL;
    }
    
    return persian_create_date(result.base.day, result.base.month, result.base.year);
}

int persian_days_between(const PersianDate* date1, const PersianDate* date2) {
    if (!date1 || !date2) return 0;
    return (int)(date2->julian_day - date1->julian_day);
}