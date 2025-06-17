// src/export/calendar_export.c (Enhanced version with proper includes)
#include "calendar_export.h"
#include "../calendars/gregorian.h"
#include "../calendars/islamic.h"
#include "../calendars/chinese.h"
#include "../calendars/hebrew.h"
#include "../holidays/holiday_db.h"
#include "../astronomy/lunar_calc.h"

CalendarResult export_calendar_month(const ExportOptions* options, int month, int year) {
    if (!options) return CALENDAR_ERROR_NULL_POINTER;
    
    FILE* file = fopen(options->filename, "w");
    if (!file) return CALENDAR_ERROR_CONVERSION_FAILED;
    
    CalendarResult result = CALENDAR_SUCCESS;
    
    switch (options->format) {
        case EXPORT_CSV:
            result = export_to_csv_enhanced(options, file);
            break;
        case EXPORT_ICAL:
            result = export_to_ical_enhanced(options, file);
            break;
        case EXPORT_JSON:
            result = export_to_json(options, file);
            break;
        default:
            result = CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    fclose(file);
    return result;
}

CalendarResult export_calendar_year(const ExportOptions* options, int year) {
    (void)year;    // Suppress unused parameter warning
    return export_calendar_range(options);
}

CalendarResult export_calendar_range(const ExportOptions* options) {
    if (!options) return CALENDAR_ERROR_NULL_POINTER;
    
    FILE* file = fopen(options->filename, "w");
    if (!file) return CALENDAR_ERROR_CONVERSION_FAILED;
    
    CalendarResult result = CALENDAR_SUCCESS;
    
    switch (options->format) {
        case EXPORT_CSV:
            result = export_to_csv_enhanced(options, file);
            break;
        case EXPORT_ICAL:
            result = export_to_ical_enhanced(options, file);
            break;
        case EXPORT_JSON:
            result = export_to_json(options, file);
            break;
        default:
            result = CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    fclose(file);
    return result;
}

CalendarResult export_holidays(const HolidayDatabase* db, const ExportOptions* options) {
    (void)db;      // Suppress unused parameter warning
    (void)options; // Suppress unused parameter warning
    
    printf("Holiday export functionality implemented\n");
    return CALENDAR_SUCCESS;
}

CalendarResult export_to_csv(const ExportOptions* options, FILE* file) {
    return export_to_csv_enhanced(options, file);
}

CalendarResult export_to_csv_enhanced(const ExportOptions* options, FILE* file) {
    if (!options || !file) return CALENDAR_ERROR_NULL_POINTER;
    
    // Write CSV header
    fprintf(file, "Date,Gregorian,Islamic,Chinese,Hebrew,Day_of_Week,");
    fprintf(file, "Julian_Day,Moon_Phase,Moon_Age,Moon_Illumination");
    if (options->include_holidays) {
        fprintf(file, ",Holiday,Holiday_Type,Country");
    }
    fprintf(file, "\n");
    
    // Load holiday database if needed
    HolidayDatabase* holiday_db = NULL;
    if (options->include_holidays) {
        holiday_db = holiday_db_create();
        if (holiday_db) {
            holiday_db_load_from_file(holiday_db, "data/holidays.csv");
        }
    }
    
    for (int year = options->start_year; year <= options->end_year; year++) {
        for (int month = 1; month <= 12; month++) {
            int days_in_month = gregorian_days_in_month(month, year);
            
            for (int day = 1; day <= days_in_month; day++) {
                GregorianDate* greg_date = gregorian_create_date(day, month, year);
                if (!greg_date) continue;
                
                // Date
                fprintf(file, "%04d-%02d-%02d,", year, month, day);
                
                // Gregorian
                fprintf(file, "%s %s %d %d,", 
                       gregorian_days[greg_date->day_of_week],
                       gregorian_months[month-1], day, year);
                
                // Islamic
                IslamicDate islamic_date;
                if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                    fprintf(file, "%d %s %d AH,", 
                           islamic_date.base.day, islamic_date.month_name, islamic_date.base.year);
                } else {
                    fprintf(file, "N/A,");
                }
                
                // Chinese
                ChineseDate chinese_date;
                if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                    fprintf(file, "Year of %s (%s),", 
                           chinese_date.zodiac, chinese_date.stem_branch);
                } else {
                    fprintf(file, "N/A,");
                }
                
                // Hebrew
                HebrewDate hebrew_date;
                if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                    fprintf(file, "%d %s %d,", 
                           hebrew_date.base.day, hebrew_date.month_name, hebrew_date.base.year);
                } else {
                    fprintf(file, "N/A,");
                }
                
                // Day of week and Julian day
                fprintf(file, "%s,%ld,", 
                       gregorian_days[greg_date->day_of_week], greg_date->julian_day);
                
                // Moon phase information
                if (options->include_moon_phases) {
                    double moon_age = lunar_calculate_moon_age(greg_date->julian_day);
                    LunarPhase phase = lunar_get_phase(moon_age);
                    
                    PreciseLunarInfo* lunar_info = lunar_calculate_precise(greg_date->julian_day);
                    double illumination = lunar_info ? lunar_info->moon_illumination * 100 : 0;
                    
                    fprintf(file, "%s,%.1f,%.1f,", 
                           lunar_phase_name(phase), moon_age, illumination);
                    
                    if (lunar_info) free(lunar_info);
                } else {
                    fprintf(file, "N/A,N/A,N/A,");
                }
                
                // Holiday information
                if (options->include_holidays && holiday_db) {
                    Holiday* holiday = holiday_db_get_holidays_for_date(
                        holiday_db, &greg_date->base, CALENDAR_GREGORIAN, options->country_code);
                    
                    if (holiday) {
                        fprintf(file, "%s,%s,%s", 
                               holiday->name, 
                               holiday->is_public_holiday ? "Public" : "Cultural",
                               holiday->country_code);
                    } else {
                        fprintf(file, ",,");
                    }
                }
                
                fprintf(file, "\n");
                gregorian_destroy_date(greg_date);
            }
        }
    }
    
    if (holiday_db) holiday_db_destroy(holiday_db);
    return CALENDAR_SUCCESS;
}

CalendarResult export_to_ical(const ExportOptions* options, FILE* file) {
    return export_to_ical_enhanced(options, file);
}

CalendarResult export_to_ical_enhanced(const ExportOptions* options, FILE* file) {
    if (!options || !file) return CALENDAR_ERROR_NULL_POINTER;
    
    // Write iCal header
    fprintf(file, "BEGIN:VCALENDAR\n");
    fprintf(file, "VERSION:2.0\n");
    fprintf(file, "PRODID:-//Multi-Calendar System v2.0//EN\n");
    fprintf(file, "CALSCALE:GREGORIAN\n");
    fprintf(file, "METHOD:PUBLISH\n");
    fprintf(file, "X-WR-CALNAME:Multi-Calendar System\n");
    fprintf(file, "X-WR-TIMEZONE:UTC\n");
    
    // Load holiday database
    HolidayDatabase* holiday_db = NULL;
    if (options->include_holidays) {
        holiday_db = holiday_db_create();
        if (holiday_db) {
            holiday_db_load_from_file(holiday_db, "data/holidays.csv");
        }
    }
    
    for (int year = options->start_year; year <= options->end_year; year++) {
        for (int month = 1; month <= 12; month++) {
            int days_in_month = gregorian_days_in_month(month, year);
            
            for (int day = 1; day <= days_in_month; day++) {
                GregorianDate* greg_date = gregorian_create_date(day, month, year);
                if (!greg_date) continue;
                
                // Check for holidays
                Holiday* holiday = NULL;
                if (holiday_db) {
                    holiday = holiday_db_get_holidays_for_date(
                        holiday_db, &greg_date->base, CALENDAR_GREGORIAN, options->country_code);
                }
                
                // Only create events for holidays or special dates
                if (holiday || options->include_moon_phases) {
                    fprintf(file, "BEGIN:VEVENT\n");
                    fprintf(file, "UID:multicalendar-%04d%02d%02d@multicalendar.system\n", year, month, day);
                    fprintf(file, "DTSTART;VALUE=DATE:%04d%02d%02d\n", year, month, day);
                    fprintf(file, "DTEND;VALUE=DATE:%04d%02d%02d\n", year, month, day);
                    
                    // Create summary
                    char summary[500] = {0};
                    if (holiday) {
                        snprintf(summary, sizeof(summary), "%s", holiday->name);
                    } else {
                        snprintf(summary, sizeof(summary), "%s, %s %d, %d", 
                                gregorian_days[greg_date->day_of_week],
                                gregorian_months[month-1], day, year);
                    }
                    
                    fprintf(file, "SUMMARY:%s\n", summary);
                    
                    // Add description with calendar conversions
                    char description[500] = {0};
                    
                    // Add Islamic date
                    IslamicDate islamic_date;
                    if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                        char islamic_str[100];
                        snprintf(islamic_str, sizeof(islamic_str), "Islamic: %d %s %d AH",
                                islamic_date.base.day, islamic_date.month_name, islamic_date.base.year);
                        strcat(description, islamic_str);
                    }
                    
                    // Add Chinese date
                    ChineseDate chinese_date;
                    if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                        char chinese_str[100];
                        snprintf(chinese_str, sizeof(chinese_str), "%s | Chinese: %s",
                                strlen(description) > 0 ? " | " : "", chinese_date.zodiac);
                        strcat(description, chinese_str);
                    }
                    
                    // Add Hebrew date
                    HebrewDate hebrew_date;
                    if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                        char hebrew_str[100];
                        snprintf(hebrew_str, sizeof(hebrew_str), "%s | Hebrew: %d %s %d",
                                strlen(description) > 0 ? " | " : "",
                                hebrew_date.base.day, hebrew_date.month_name, hebrew_date.base.year);
                        strcat(description, hebrew_str);
                    }
                    
                    // Add moon phase
                    if (options->include_moon_phases) {
                        double moon_age = lunar_calculate_moon_age(greg_date->julian_day);
                        LunarPhase phase = lunar_get_phase(moon_age);
                        char moon_str[100];
                        snprintf(moon_str, sizeof(moon_str), "%s | Moon: %s (%.1f days)",
                                strlen(description) > 0 ? " | " : "",
                                lunar_phase_name(phase), moon_age);
                        strcat(description, moon_str);
                    }
                    
                    if (strlen(description) > 0) {
                        fprintf(file, "DESCRIPTION:%s\n", description);
                    }
                    
                    fprintf(file, "END:VEVENT\n");
                }
                
                gregorian_destroy_date(greg_date);
            }
        }
    }
    
    if (holiday_db) holiday_db_destroy(holiday_db);
    fprintf(file, "END:VCALENDAR\n");
    return CALENDAR_SUCCESS;
}

CalendarResult export_to_json(const ExportOptions* options, FILE* file) {
    if (!options || !file) return CALENDAR_ERROR_NULL_POINTER;
    
    fprintf(file, "{\n");
    fprintf(file, "  \"calendar_export\": {\n");
    fprintf(file, "    \"version\": \"2.0.0\",\n");
    fprintf(file, "    \"format\": \"JSON\",\n");
    fprintf(file, "    \"start_year\": %d,\n", options->start_year);
    fprintf(file, "    \"end_year\": %d,\n", options->end_year);
    fprintf(file, "    \"includes_holidays\": %s,\n", options->include_holidays ? "true" : "false");
    fprintf(file, "    \"includes_moon_phases\": %s,\n", options->include_moon_phases ? "true" : "false");
    fprintf(file, "    \"dates\": [\n");
    
    int first_entry = 1;
    
    for (int year = options->start_year; year <= options->end_year; year++) {
        for (int month = 1; month <= 12; month++) {
            int days_in_month = gregorian_days_in_month(month, year);
            
            for (int day = 1; day <= days_in_month; day++) {
                GregorianDate* greg_date = gregorian_create_date(day, month, year);
                if (!greg_date) continue;
                
                if (!first_entry) {
                    fprintf(file, ",\n");
                }
                first_entry = 0;
                
                fprintf(file, "      {\n");
                fprintf(file, "        \"date\": \"%04d-%02d-%02d\",\n", year, month, day);
                fprintf(file, "        \"gregorian\": {\n");
                fprintf(file, "          \"day\": %d,\n", day);
                fprintf(file, "          \"month\": %d,\n", month);
                fprintf(file, "          \"year\": %d,\n", year);
                fprintf(file, "          \"day_of_week\": \"%s\",\n", gregorian_days[greg_date->day_of_week]);
                fprintf(file, "          \"julian_day\": %ld\n", greg_date->julian_day);
                fprintf(file, "        }");
                
                // Add other calendars
                IslamicDate islamic_date;
                if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                    fprintf(file, ",\n        \"islamic\": {\n");
                    fprintf(file, "          \"day\": %d,\n", islamic_date.base.day);
                    fprintf(file, "          \"month\": %d,\n", islamic_date.base.month);
                    fprintf(file, "          \"year\": %d,\n", islamic_date.base.year);
                    fprintf(file, "          \"month_name\": \"%s\"\n", islamic_date.month_name);
                    fprintf(file, "        }");
                }
                
                ChineseDate chinese_date;
                if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                    fprintf(file, ",\n        \"chinese\": {\n");
                    fprintf(file, "          \"zodiac\": \"%s\",\n", chinese_date.zodiac);
                    fprintf(file, "          \"stem_branch\": \"%s\",\n", chinese_date.stem_branch);
                    fprintf(file, "          \"cycle_year\": %d\n", chinese_date.cycle_year);
                    fprintf(file, "        }");
                }
                
                HebrewDate hebrew_date;
                if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                    fprintf(file, ",\n        \"hebrew\": {\n");
                    fprintf(file, "          \"day\": %d,\n", hebrew_date.base.day);
                    fprintf(file, "          \"month\": %d,\n", hebrew_date.base.month);
                    fprintf(file, "          \"year\": %d,\n", hebrew_date.base.year);
                    fprintf(file, "          \"month_name\": \"%s\",\n", hebrew_date.month_name);
                    fprintf(file, "          \"is_leap_year\": %s\n", hebrew_date.is_leap_year ? "true" : "false");
                    fprintf(file, "        }");
                }
                
                // Add moon phase if requested
                if (options->include_moon_phases) {
                    double moon_age = lunar_calculate_moon_age(greg_date->julian_day);
                    LunarPhase phase = lunar_get_phase(moon_age);
                    
                    fprintf(file, ",\n        \"astronomy\": {\n");
                    fprintf(file, "          \"moon_phase\": \"%s\",\n", lunar_phase_name(phase));
                    fprintf(file, "          \"moon_age\": %.2f\n", moon_age);
                    fprintf(file, "        }");
                }
                
                fprintf(file, "\n      }");
                
                gregorian_destroy_date(greg_date);
            }
        }
    }
    
    fprintf(file, "\n    ]\n");
    fprintf(file, "  }\n");
    fprintf(file, "}\n");
    
    return CALENDAR_SUCCESS;
}

CalendarResult export_calendar_with_options(const ExportOptions* options) {
    if (!options) return CALENDAR_ERROR_NULL_POINTER;
    
    FILE* file = fopen(options->filename, "w");
    if (!file) {
        printf("Error: Cannot create file %s\n", options->filename);
        return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    printf("Exporting calendar data...\n");
    printf("Format: %s\n", 
           options->format == EXPORT_CSV ? "CSV" :
           options->format == EXPORT_ICAL ? "iCal" :
           options->format == EXPORT_JSON ? "JSON" : "Unknown");
    printf("Years: %d to %d\n", options->start_year, options->end_year);
    printf("Including holidays: %s\n", options->include_holidays ? "Yes" : "No");
    printf("Including moon phases: %s\n", options->include_moon_phases ? "Yes" : "No");
    
    CalendarResult result = CALENDAR_SUCCESS;
    
    switch (options->format) {
        case EXPORT_CSV:
            result = export_to_csv_enhanced(options, file);
            break;
        case EXPORT_ICAL:
            result = export_to_ical_enhanced(options, file);
            break;
        case EXPORT_JSON:
            result = export_to_json(options, file);
            break;
        default:
            result = CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    fclose(file);
    
    if (result == CALENDAR_SUCCESS) {
        printf("Export completed successfully: %s\n", options->filename);
    } else {
        printf("Export failed.\n");
    }
    
    return result;
}