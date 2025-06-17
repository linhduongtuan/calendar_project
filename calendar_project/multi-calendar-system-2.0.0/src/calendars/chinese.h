#ifndef CHINESE_H
#define CHINESE_H

#include "../../include/calendar_types.h"

// Constants
#define CHINESE_ZODIAC_COUNT 12
#define CHINESE_STEMS_COUNT 10
#define CHINESE_BRANCHES_COUNT 12

// External arrays
extern const char* chinese_zodiac[CHINESE_ZODIAC_COUNT];
extern const char* heavenly_stems[CHINESE_STEMS_COUNT];
extern const char* earthly_branches[CHINESE_BRANCHES_COUNT];

// Core functions
const char* chinese_get_zodiac(int year);
void chinese_get_stem_branch(int year, char* result, size_t buffer_size);
int chinese_get_cycle_year(int year);

// Date creation and manipulation
ChineseDate* chinese_create_date(int day, int month, int year);
void chinese_destroy_date(ChineseDate* date);

// Display functions
void chinese_print_date(const ChineseDate* date);

// Conversion functions (simplified)
CalendarResult chinese_from_gregorian(const GregorianDate* greg_date, ChineseDate* chinese_date);

#endif // CHINESE_H