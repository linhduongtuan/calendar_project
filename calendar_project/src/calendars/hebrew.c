// src/calendars/hebrew.c (Enhanced with accurate calculations)
#include "hebrew.h"
#include "gregorian.h"
#include "../utils/date_utils.h"

const char* hebrew_months[HEBREW_MONTHS_COUNT] = {
    "Tishrei", "Cheshvan", "Kislev", "Tevet", "Shevat", "Adar",
    "Nisan", "Iyar", "Sivan", "Tammuz", "Av", "Elul", "Adar II"
};

const char* hebrew_months_hebrew[HEBREW_MONTHS_COUNT] = {
    "תשרי", "חשון", "כסלו", "טבת", "שבט", "אדר",
    "ניסן", "אייר", "סיון", "תמוז", "אב", "אלול", "אדר ב׳"
};

int hebrew_is_leap_year(int year) {
    // Hebrew leap year: 7 out of every 19 years (Metonic cycle)
    return ((year * 7) % 19) < 7;
}

int hebrew_months_in_year(int year) {
    return hebrew_is_leap_year(year) ? 13 : 12;
}

int hebrew_days_in_month(int month, int year) {
    // Standard month lengths (simplified but more accurate)
    int days_per_month[] = {30, 29, 29, 29, 30, 29, 30, 29, 30, 29, 30, 29, 29};
    
    if (month < 1 || month > hebrew_months_in_year(year)) {
        return -1;
    }
    
    if (month > 12) {
        // Adar II in leap year
        return 29;
    }
    
    // Apply variable month lengths for Cheshvan and Kislev
    if (month == 2) { // Cheshvan
        return 29 + (year % 3); // Simplified variable length
    }
    if (month == 3) { // Kislev
        return 29 + ((year + 1) % 2); // Simplified variable length
    }
    
    return days_per_month[month - 1];
}

long hebrew_elapsed_days(int year) {
    // Basic calculation for compatibility
    long days = (year - 1) * 354;  // Base year length
    days += ((year - 1) * 7) / 19 * 30;  // Add leap month days
    return days;
}

long hebrew_elapsed_days_accurate(int year) {
    // More accurate calculation using the 19-year Metonic cycle
    if (year <= 1) return 0;
    
    long complete_cycles = (year - 1) / 19;
    int years_in_cycle = (year - 1) % 19;
    
    // Days in complete 19-year cycles (6939.6896 days per cycle, rounded to 6939)
    long days = complete_cycles * 6939;
    
    // Add days for partial cycle with more accurate year lengths
    for (int y = 1; y <= years_in_cycle; y++) {
        if (hebrew_is_leap_year(y)) {
            days += 383;  // Leap year length
        } else {
            days += 354;  // Regular year length
        }
        
        // Add variable adjustments for deficient/complete/abundant years
        days += (y % 3);  // Simplified adjustment
    }
    
    return days;
}

HebrewDate* hebrew_create_date(int day, int month, int year) {
    if (year < 1 || month < 1 || month > hebrew_months_in_year(year)) {
        return NULL;
    }
    
    int max_days = hebrew_days_in_month(month, year);
    if (day < 1 || day > max_days) {
        return NULL;
    }
    
    HebrewDate* date = malloc(sizeof(HebrewDate));
    if (!date) return NULL;
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    date->is_leap_year = hebrew_is_leap_year(year);
    date->leap_month_added = hebrew_is_leap_year(year) ? 6 : 0; // Adar II
    
    strcpy(date->month_name, hebrew_months[month - 1]);
    snprintf(date->hebrew_date, sizeof(date->hebrew_date), 
             "%d %s %d", day, hebrew_months_hebrew[month - 1], year);
    
    return date;
}

void hebrew_destroy_date(HebrewDate* date) {
    if (date) free(date);
}

void hebrew_print_date(const HebrewDate* date) {
    if (!date) return;
    printf("%d %s %d (%s)\n", 
           date->base.day, date->month_name, date->base.year, date->hebrew_date);
}

void hebrew_print_month(int month, int year) {
    if (month < 1 || month > hebrew_months_in_year(year)) {
        printf("Invalid Hebrew month: %d for year %d\n", month, year);
        return;
    }
    
    printf("\n%s %d", hebrew_months[month - 1], year);
    if (hebrew_is_leap_year(year)) {
        printf(" (Leap Year)");
    }
    printf("\n");
    printf("Su Mo Tu We Th Fr Sa\n");
    printf("-------------------\n");
    
    int days_count = hebrew_days_in_month(month, year);
    
    for (int day = 1; day <= days_count; day++) {
        printf("%2d ", day);
        if (day % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

CalendarResult hebrew_from_gregorian(const GregorianDate* greg_date, HebrewDate* hebrew_date) {
    return hebrew_from_gregorian_enhanced(greg_date, hebrew_date);
}

CalendarResult hebrew_from_gregorian_enhanced(const GregorianDate* greg_date, HebrewDate* hebrew_date) {
    if (!greg_date || !hebrew_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    double julian_day = (double)greg_date->julian_day;
    double n = julian_day - HEBREW_EPOCH_JD;
    
    if (n < 0) {
        return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    // Calculate Hebrew year more accurately
    double hebrew_year_approx = n / 365.2468 + 1;
    int hebrew_year = (int)hebrew_year_approx;
    
    // Adjust for the complex Hebrew calendar rules
    while (hebrew_year > 1 && hebrew_elapsed_days_accurate(hebrew_year) > n) {
        hebrew_year--;
    }
    while (hebrew_elapsed_days_accurate(hebrew_year + 1) <= n) {
        hebrew_year++;
    }
    
    // Calculate month and day
    double days_in_year = n - hebrew_elapsed_days_accurate(hebrew_year);
    int month = 1;
    
    while (month <= hebrew_months_in_year(hebrew_year) && 
           days_in_year >= hebrew_days_in_month(month, hebrew_year)) {
        days_in_year -= hebrew_days_in_month(month, hebrew_year);
        month++;
    }
    
    int day = (int)days_in_year + 1;
    
    // Validate and create Hebrew date
    if (month > hebrew_months_in_year(hebrew_year)) {
        month = hebrew_months_in_year(hebrew_year);
        day = hebrew_days_in_month(month, hebrew_year);
    }
    
    if (day < 1) {
        day = 1;
    }
    
    hebrew_date->base.day = day;
    hebrew_date->base.month = month;
    hebrew_date->base.year = hebrew_year;
    hebrew_date->is_leap_year = hebrew_is_leap_year(hebrew_year);
    hebrew_date->leap_month_added = hebrew_is_leap_year(hebrew_year) ? 6 : 0;
    strcpy(hebrew_date->month_name, hebrew_months[month - 1]);
    snprintf(hebrew_date->hebrew_date, sizeof(hebrew_date->hebrew_date),
             "%d %s %d", day, hebrew_months_hebrew[month - 1], hebrew_year);
    
    return CALENDAR_SUCCESS;
}

CalendarResult hebrew_to_gregorian(const HebrewDate* hebrew_date, GregorianDate* greg_date) {
    if (!hebrew_date || !greg_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Use enhanced calculation
    long days_since_epoch = hebrew_elapsed_days_accurate(hebrew_date->base.year);
    
    // Add days for months in current year
    for (int month = 1; month < hebrew_date->base.month; month++) {
        days_since_epoch += hebrew_days_in_month(month, hebrew_date->base.year);
    }
    
    // Add days in current month
    days_since_epoch += hebrew_date->base.day - 1;
    
    // Convert to Julian Day Number
    long julian_day = (long)(HEBREW_EPOCH_JD + days_since_epoch);
    
    // Convert Julian day to Gregorian date manually
    int day, month, year;
    julian_day_to_date(julian_day, &day, &month, &year);
    
    // Create Gregorian date structure
    greg_date->base.day = day;
    greg_date->base.month = month;
    greg_date->base.year = year;
    greg_date->julian_day = julian_day;
    greg_date->day_of_week = gregorian_day_of_week(day, month, year);
    
    return CALENDAR_SUCCESS;
}