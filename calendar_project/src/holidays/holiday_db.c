// src/holidays/holiday_db.c (Fixed with proper includes)
#include "holiday_db.h"
#include "../utils/string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HolidayDatabase* holiday_db_create(void) {
    HolidayDatabase* db = malloc(sizeof(HolidayDatabase));
    if (!db) return NULL;
    
    db->holidays = malloc(sizeof(Holiday) * MAX_HOLIDAYS);
    if (!db->holidays) {
        free(db);
        return NULL;
    }
    
    db->count = 0;
    return db;
}

void holiday_db_destroy(HolidayDatabase* db) {
    if (db) {
        if (db->holidays) {
            free(db->holidays);
        }
        free(db);
    }
}

CalendarResult holiday_db_load_from_file(HolidayDatabase* db, const char* filename) {
    if (!db || !filename) return CALENDAR_ERROR_NULL_POINTER;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Holiday database file not found: %s\n", filename);
        printf("Creating sample holiday database...\n");
        
        // Create directory if it doesn't exist
        char dir_command[256];
        snprintf(dir_command, sizeof(dir_command), "mkdir -p %s", 
                strstr(filename, "/") ? "data" : ".");
        system(dir_command);
        
        // Create sample data if file doesn't exist
        file = fopen(filename, "w");
        if (file) {
            fprintf(file, "Name,Description,CalendarType,HolidayType,Month,Day,Country,IsPublic\n");
            fprintf(file, "New Year's Day,Beginning of the year,0,0,1,1,US,1\n");
            fprintf(file, "Martin Luther King Jr Day,Civil Rights Leader Birthday,0,4,1,15,US,1\n");
            fprintf(file, "Independence Day,US Independence Day,0,0,7,4,US,1\n");
            fprintf(file, "Christmas Day,Christian celebration,0,0,12,25,US,1\n");
            fprintf(file, "Chinese New Year,Lunar New Year,2,0,1,1,CN,1\n");
            fprintf(file, "Eid al-Fitr,End of Ramadan,1,0,10,1,SA,1\n");
            fprintf(file, "Rosh Hashanah,Jewish New Year,3,0,1,1,IL,1\n");
            fprintf(file, "Yom Kippur,Day of Atonement,3,0,1,10,IL,1\n");
            fclose(file);
            
            // Reopen for reading
            file = fopen(filename, "r");
        }
        
        if (!file) return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    char line[512];
    // int line_count = 0; // Unused variable
    
    // Skip header line
    if (fgets(line, sizeof(line), file)) {
        // Header line processed
    }
    
    while (fgets(line, sizeof(line), file) && db->count < MAX_HOLIDAYS) {
        // Process data line
        
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Parse CSV line
        char* tokens[8];
        int token_count = 0;
        char* line_copy = strdup(line);
        char* token = strtok(line_copy, ",");
        
        while (token && token_count < 8) {
            tokens[token_count++] = token;
            token = strtok(NULL, ",");
        }
        
        if (token_count >= 8) {
            Holiday* holiday = &db->holidays[db->count];
            
            strncpy(holiday->name, tokens[0], sizeof(holiday->name) - 1);
            holiday->name[sizeof(holiday->name) - 1] = '\0';
            
            strncpy(holiday->description, tokens[1], sizeof(holiday->description) - 1);
            holiday->description[sizeof(holiday->description) - 1] = '\0';
            
            holiday->calendar_type = (CalendarType)atoi(tokens[2]);
            holiday->holiday_type = atoi(tokens[3]);
            holiday->date.month = atoi(tokens[4]);
            holiday->date.day = atoi(tokens[5]);
            holiday->date.year = 0; // Default for recurring holidays
            
            strncpy(holiday->country_code, tokens[6], sizeof(holiday->country_code) - 1);
            holiday->country_code[sizeof(holiday->country_code) - 1] = '\0';
            
            holiday->is_public_holiday = atoi(tokens[7]);
            
            db->count++;
        }
        
        free(line_copy);
    }
    
    fclose(file);
    
    printf("Loaded %d holidays from %s\n", db->count, filename);
    return CALENDAR_SUCCESS;
}

CalendarResult holiday_db_add_holiday(HolidayDatabase* db, const Holiday* holiday) {
    if (!db || !holiday || db->count >= MAX_HOLIDAYS) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    db->holidays[db->count] = *holiday;
    db->count++;
    
    return CALENDAR_SUCCESS;
}

Holiday* holiday_db_get_holidays_for_date(HolidayDatabase* db, const Date* date, 
                                          CalendarType calendar_type, const char* country_code) {
    if (!db || !date) return NULL;
    
    for (int i = 0; i < db->count; i++) {
        Holiday* holiday = &db->holidays[i];
        
        // Check calendar type
        if (holiday->calendar_type != calendar_type) continue;
        
        // Check country code if specified
        if (country_code && strlen(country_code) > 0) {
            if (strcmp(holiday->country_code, country_code) != 0) continue;
        }
        
        // Check date match
        if (holiday->date.month == date->month && holiday->date.day == date->day) {
            return holiday;
        }
    }
    
    return NULL;
}

void holiday_db_print_holidays(const HolidayDatabase* db) {
    if (!db) return;
    
    printf("\n=== HOLIDAY DATABASE ===\n");
    printf("Total holidays: %d\n\n", db->count);
    
    for (int i = 0; i < db->count; i++) {
        const Holiday* holiday = &db->holidays[i];
        printf("%s (%s)\n", holiday->name, holiday->country_code);
        printf("  Date: %d/%d\n", holiday->date.day, holiday->date.month);
        printf("  Type: %s\n", holiday->is_public_holiday ? "Public" : "Cultural");
        printf("  Calendar: %s\n", 
               holiday->calendar_type == CALENDAR_GREGORIAN ? "Gregorian" :
               holiday->calendar_type == CALENDAR_ISLAMIC ? "Islamic" :
               holiday->calendar_type == CALENDAR_CHINESE ? "Chinese" :
               holiday->calendar_type == CALENDAR_HEBREW ? "Hebrew" : "Unknown");
        printf("  Description: %s\n\n", holiday->description);
    }
}

void holiday_db_print_holidays_for_month(const HolidayDatabase* db, int month, 
                                         CalendarType calendar_type, const char* country_code) {
    if (!db) return;
    
    printf("\n=== HOLIDAYS FOR MONTH %d ===\n", month);
    
    int found = 0;
    for (int i = 0; i < db->count; i++) {
        const Holiday* holiday = &db->holidays[i];
        
        // Check calendar type
        if (holiday->calendar_type != calendar_type) continue;
        
        // Check country code if specified
        if (country_code && strlen(country_code) > 0) {
            if (strcmp(holiday->country_code, country_code) != 0) continue;
        }
        
        // Check month
        if (holiday->date.month == month) {
            printf("%s - %d/%d (%s)\n", 
                   holiday->name, holiday->date.day, holiday->date.month,
                   holiday->is_public_holiday ? "Public" : "Cultural");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No holidays found for this month.\n");
    }
    printf("\n");
}

CalendarResult holiday_db_save_to_file(const HolidayDatabase* db, const char* filename) {
    if (!db || !filename) return CALENDAR_ERROR_NULL_POINTER;
    
    FILE* file = fopen(filename, "w");
    if (!file) return CALENDAR_ERROR_CONVERSION_FAILED;
    
    // Write header
    fprintf(file, "Name,Description,CalendarType,HolidayType,Month,Day,Country,IsPublic\n");
    
    // Write holidays
    for (int i = 0; i < db->count; i++) {
        const Holiday* holiday = &db->holidays[i];
        fprintf(file, "%s,%s,%d,%d,%d,%d,%s,%d\n",
                holiday->name, holiday->description,
                holiday->calendar_type, holiday->holiday_type,
                holiday->date.month, holiday->date.day,
                holiday->country_code, holiday->is_public_holiday);
    }
    
    fclose(file);
    printf("Saved %d holidays to %s\n", db->count, filename);
    return CALENDAR_SUCCESS;
}