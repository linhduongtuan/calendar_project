// src/holidays/holiday_db.h (Updated with proper includes)
#ifndef HOLIDAY_DB_H
#define HOLIDAY_DB_H

#include "../../include/calendar_types.h"

#define MAX_HOLIDAYS 1000

// Holiday types
typedef enum {
    HOLIDAY_FIXED,       // Fixed date every year
    HOLIDAY_EASTER,      // Easter-based calculation
    HOLIDAY_LUNAR,       // Lunar calendar based
    HOLIDAY_SOLAR,       // Solar calendar based
    HOLIDAY_WEEKDAY      // Nth weekday of month
} HolidayType;

// Holiday structure
typedef struct {
    char name[64];
    char description[128];
    CalendarType calendar_type;
    int holiday_type;
    Date date;
    char country_code[4];
    int is_public_holiday;
} Holiday;

// Holiday database structure
typedef struct {
    Holiday* holidays;
    int count;
} HolidayDatabase;

// Database management functions
HolidayDatabase* holiday_db_create(void);
void holiday_db_destroy(HolidayDatabase* db);

// Loading and saving
CalendarResult holiday_db_load_from_file(HolidayDatabase* db, const char* filename);
CalendarResult holiday_db_save_to_file(const HolidayDatabase* db, const char* filename);

// Holiday management
CalendarResult holiday_db_add_holiday(HolidayDatabase* db, const Holiday* holiday);
Holiday* holiday_db_get_holidays_for_date(HolidayDatabase* db, const Date* date, 
                                          CalendarType calendar_type, const char* country_code);

// Display functions
void holiday_db_print_holidays(const HolidayDatabase* db);
void holiday_db_print_holidays_for_month(const HolidayDatabase* db, int month, 
                                         CalendarType calendar_type, const char* country_code);

#endif // HOLIDAY_DB_H