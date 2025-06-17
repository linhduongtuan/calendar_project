#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include "../../include/calendar_types.h"

// Julian Day Number utilities
long julian_day_from_date(int day, int month, int year);
void julian_day_to_date(long jdn, int* day, int* month, int* year);

// String formatting utilities
void format_date_string(const Date* date, CalendarType type, char* buffer, size_t buffer_size);
CalendarResult parse_date_string(const char* date_str, Date* result);

// Mathematical utilities
int modulo(int a, int b);
double fractional_part(double x);

// Validation utilities
int is_valid_day(int day);
int is_valid_month(int month);
int is_valid_year(int year);

// Date comparison utilities
int compare_dates(const Date* date1, const Date* date2);
int days_between_dates(const Date* start_date, const Date* end_date);

#endif // DATE_UTILS_H