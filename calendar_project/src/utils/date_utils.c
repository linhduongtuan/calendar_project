#include "date_utils.h"

long julian_day_from_date(int day, int month, int year) {
    if (month <= 2) {
        year--;
        month += 12;
    }
    
    int a = year / 100;
    int b = 2 - a + (a / 4);
    
    return (long)(365.25 * (year + 4716)) + 
           (long)(30.6001 * (month + 1)) + 
           day + b - 1524;
}

void julian_day_to_date(long jdn, int* day, int* month, int* year) {
    long a = jdn + 32044;
    long b = (4 * a + 3) / 146097;
    long c = a - (146097 * b) / 4;
    long d = (4 * c + 3) / 1461;
    long e = c - (1461 * d) / 4;
    long m = (5 * e + 2) / 153;
    
    *day = e - (153 * m + 2) / 5 + 1;
    *month = m + 3 - 12 * (m / 10);
    *year = 100 * b + d - 4800 + m / 10;
}

int modulo(int a, int b) {
    int result = a % b;
    return result < 0 ? result + b : result;
}

double fractional_part(double x) {
    return x - floor(x);
}

int is_valid_day(int day) {
    return day >= 1 && day <= 31;
}

int is_valid_month(int month) {
    return month >= 1 && month <= 12;
}

int is_valid_year(int year) {
    return year >= 1;
}

void format_date_string(const Date* date, CalendarType type, char* buffer, size_t buffer_size) {
    if (!date || !buffer) return;
    
    switch (type) {
        case CALENDAR_GREGORIAN:
            snprintf(buffer, buffer_size, "%04d-%02d-%02d", 
                    date->year, date->month, date->day);
            break;
        case CALENDAR_ISLAMIC:
            snprintf(buffer, buffer_size, "%d/%d/%d AH", 
                    date->day, date->month, date->year);
            break;
        case CALENDAR_CHINESE:
            snprintf(buffer, buffer_size, "%d-%d-%d (Chinese)", 
                    date->year, date->month, date->day);
            break;
        default:
            snprintf(buffer, buffer_size, "%d-%d-%d", 
                    date->year, date->month, date->day);
    }
}

int compare_dates(const Date* date1, const Date* date2) {
    if (!date1 || !date2) return 0;
    
    if (date1->year != date2->year) {
        return date1->year - date2->year;
    }
    if (date1->month != date2->month) {
        return date1->month - date2->month;
    }
    return date1->day - date2->day;
}

CalendarResult parse_date_string(const char* date_str, Date* result) {
    if (!date_str || !result) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    int parsed = sscanf(date_str, "%d-%d-%d", &result->year, &result->month, &result->day);
    if (parsed != 3) {
        parsed = sscanf(date_str, "%d/%d/%d", &result->day, &result->month, &result->year);
    }
    
    if (parsed != 3) {
        return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    if (!is_valid_year(result->year) || !is_valid_month(result->month) || !is_valid_day(result->day)) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    return CALENDAR_SUCCESS;
}