#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include "../../include/calendar_types.h"
#include <limits.h>  // Add this include for INT_MAX and INT_MIN

// String manipulation utilities
char* string_trim(char* str);
char* string_trim_copy(const char* str);
int string_is_empty(const char* str);
int string_compare_ignore_case(const char* str1, const char* str2);

// String parsing utilities
int string_split(const char* str, char delimiter, char** tokens, int max_tokens);
char* string_find_substring(const char* haystack, const char* needle);
int string_starts_with(const char* str, const char* prefix);
int string_ends_with(const char* str, const char* suffix);  // Fixed: was "const suffix"

// String formatting utilities
void string_to_upper(char* str);
void string_to_lower(char* str);
void string_capitalize(char* str);

// Number parsing utilities
int string_to_int(const char* str, int* result);
int string_to_double(const char* str, double* result);
int string_is_numeric(const char* str);

// Date string utilities
CalendarResult string_parse_date_components(const char* date_str, int* day, int* month, int* year);
void string_format_date_iso(int day, int month, int year, char* buffer, size_t buffer_size);
void string_format_date_readable(int day, int month, int year, const char* month_names[], char* buffer, size_t buffer_size);

// Calendar-specific string utilities
CalendarResult string_parse_islamic_date(const char* date_str, int* day, int* month, int* year);
CalendarResult string_parse_chinese_date(const char* date_str, int* day, int* month, int* year, int* is_leap);

// File reading utilities for calendar data
CalendarResult string_read_calendar_data_line(const char* line, char* type, char* data[], int max_fields);
int string_is_comment_line(const char* line);

// Unicode/UTF-8 utilities for international calendar names
int string_utf8_length(const char* str);
int string_is_valid_utf8(const char* str);

// Buffer safety utilities
size_t string_safe_copy(char* dest, const char* src, size_t dest_size);
size_t string_safe_concat(char* dest, const char* src, size_t dest_size);

#endif // STRING_UTILS_H