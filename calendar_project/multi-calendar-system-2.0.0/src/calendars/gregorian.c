// src/calendars/gregorian.c (Fixed - replace the last function)
#include "gregorian.h"
#include "../utils/date_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* gregorian_months[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

const char* gregorian_days[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

// Fixed Zeller's congruence for accurate day-of-week calculation
static int calculate_day_of_week(int day, int month, int year) {
    // Zeller's congruence algorithm
    int q = day;
    int m = month;
    int k = year % 100;
    int j = year / 100;
    
    // For Zeller's congruence, January and February are counted as months 13 and 14 of the previous year
    if (m < 3) {
        m += 12;
        if (k == 0) {
            k = 99;
            j--;
        } else {
            k--;
        }
    }
    
    // Zeller's formula
    int h = (q + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) - 2 * j) % 7;
    
    // Convert Zeller's result (0=Saturday) to standard (0=Sunday)
    return (h + 6) % 7;
}

GregorianDate* gregorian_create_date(int day, int month, int year) {
    if (gregorian_validate_date(day, month, year) != CALENDAR_SUCCESS) {
        return NULL;
    }
    
    GregorianDate* date = malloc(sizeof(GregorianDate));
    if (!date) return NULL;
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    
    // Calculate Julian Day Number using the standard formula
    date->julian_day = gregorian_to_julian_day(day, month, year);
    
    // Calculate day of week using the corrected algorithm
    date->day_of_week = calculate_day_of_week(day, month, year);
    
    return date;
}

void gregorian_destroy_date(GregorianDate* date) {
    if (date) {
        free(date);
    }
}

CalendarResult gregorian_validate_date(int day, int month, int year) {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    int max_days = gregorian_days_in_month(month, year);
    if (day > max_days) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    return CALENDAR_SUCCESS;
}

int gregorian_is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int gregorian_days_in_month(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && gregorian_is_leap_year(year)) {
        return 29;
    }
    
    return days[month - 1];
}

long gregorian_to_julian_day(int day, int month, int year) {
    // Standard Julian Day calculation
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12 * a - 3;
    
    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 + 1721119;
}

int gregorian_day_of_week(int day, int month, int year) {
    return calculate_day_of_week(day, month, year);
}

void gregorian_print_date(const GregorianDate* date) {
    if (!date) return;
    
    printf("%s, %s %d, %d\n", 
           gregorian_days[date->day_of_week],
           gregorian_months[date->base.month - 1], 
           date->base.day, 
           date->base.year);
}

void gregorian_print_month(int month, int year) {
    if (month < 1 || month > 12 || year < 1) {
        printf("Invalid month or year\n");
        return;
    }
    
    printf("\n%s %d\n", gregorian_months[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");
    
    // Get the first day of the month
    GregorianDate* first_day = gregorian_create_date(1, month, year);
    if (!first_day) return;
    
    int start_day = first_day->day_of_week;
    int days_in_month = gregorian_days_in_month(month, year);
    
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
    
    gregorian_destroy_date(first_day);
}

GregorianDate* gregorian_add_days(const GregorianDate* date, int days) {
    if (!date) return NULL;
    
    long new_julian = date->julian_day + days;
    
    // Convert back to Gregorian
    int new_day, new_month, new_year;
    julian_day_to_date(new_julian, &new_day, &new_month, &new_year);
    
    return gregorian_create_date(new_day, new_month, new_year);
}

int gregorian_days_between(const GregorianDate* date1, const GregorianDate* date2) {
    if (!date1 || !date2) return 0;
    return (int)(date2->julian_day - date1->julian_day);
}

GregorianDate* gregorian_today() {
    // This would typically get the system date, but for now return a default
    // In a real implementation, you'd use system calls to get current date
    return gregorian_create_date(16, 6, 2025); // Current date based on context
}

CalendarResult gregorian_from_julian_day(long jdn, GregorianDate* result) {
    if (!result) return CALENDAR_ERROR_INVALID_DATE;
    
    int day, month, year;
    julian_day_to_date(jdn, &day, &month, &year);
    
    GregorianDate* temp = gregorian_create_date(day, month, year);
    if (!temp) return CALENDAR_ERROR_INVALID_DATE;
    
    *result = *temp;
    gregorian_destroy_date(temp);
    
    return CALENDAR_SUCCESS;
}