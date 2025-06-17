// src/export/calendar_export.h (Updated)
#ifndef CALENDAR_EXPORT_H
#define CALENDAR_EXPORT_H

#include "../../include/calendar_types.h"
#include "../holidays/holiday_db.h"

// Export formats
typedef enum {
    EXPORT_CSV,
    EXPORT_ICAL,
    EXPORT_JSON,
    EXPORT_XML
} ExportFormat;

// Export options
typedef struct {
    ExportFormat format;
    CalendarType calendar_type;
    int start_year;
    int end_year;
    int include_holidays;
    int include_moon_phases;
    char country_code[5];
    char filename[256];
} ExportOptions;

// Export functions
CalendarResult export_calendar_month(const ExportOptions* options, int month, int year);
CalendarResult export_calendar_year(const ExportOptions* options, int year);
CalendarResult export_calendar_range(const ExportOptions* options);
CalendarResult export_holidays(const HolidayDatabase* db, const ExportOptions* options);

// Format-specific functions
CalendarResult export_to_csv(const ExportOptions* options, FILE* file);
CalendarResult export_to_csv_enhanced(const ExportOptions* options, FILE* file);
CalendarResult export_to_ical(const ExportOptions* options, FILE* file);
CalendarResult export_to_ical_enhanced(const ExportOptions* options, FILE* file);
CalendarResult export_to_json(const ExportOptions* options, FILE* file);

#endif // CALENDAR_EXPORT_H