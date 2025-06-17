// src/main.c (Updated with better error handling)
#include <stdio.h>
#include <time.h>
#include "calendars/gregorian.h"
#include "calendars/islamic.h"
#include "calendars/chinese.h"
#include "calendars/hebrew.h"
#include "ui/console_ui.h"
#include "utils/date_utils.h"

void print_header() {
    time_t current_time;
    struct tm *utc_time;
    
    time(&current_time);
    utc_time = gmtime(&current_time);
    
    printf("Current Date and Time (UTC - YYYY-MM-DD HH:MM:SS formatted): ");
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",
           utc_time->tm_year + 1900,
           utc_time->tm_mon + 1,
           utc_time->tm_mday,
           utc_time->tm_hour,
           utc_time->tm_min,
           utc_time->tm_sec);
    
    printf("Current User's Login: linhduongtuan\n\n");
}

void demonstrate_calendars() {
    printf("=== Multi-Calendar System v2.0 ===\n");
    printf("Supports: Gregorian, Islamic, Chinese, Hebrew\n");
    printf("Features: Astronomy, Holidays, Timezones, Export\n\n");
    
    // Show current date in all calendars
    time_t now = time(NULL);
    struct tm *current = localtime(&now);
    
    GregorianDate* greg_today = gregorian_create_date(
        current->tm_mday,
        current->tm_mon + 1,
        current->tm_year + 1900
    );
    
    if (greg_today) {
        printf("Today (Gregorian): ");
        gregorian_print_date(greg_today);
        
        // Convert to other calendars
        IslamicDate islamic_today;
        if (islamic_from_gregorian(greg_today, &islamic_today) == CALENDAR_SUCCESS) {
            printf("Today (Islamic): ");
            islamic_print_date(&islamic_today);
        }
        
        ChineseDate chinese_today;
        if (chinese_from_gregorian(greg_today, &chinese_today) == CALENDAR_SUCCESS) {
            printf("Today (Chinese): ");
            chinese_print_date(&chinese_today);
        }
        
        HebrewDate hebrew_today;
        if (hebrew_from_gregorian(greg_today, &hebrew_today) == CALENDAR_SUCCESS) {
            printf("Today (Hebrew): ");
            hebrew_print_date(&hebrew_today);
        } else {
            printf("Today (Hebrew): Conversion not available\n");
        }
        
        printf("\n");
        gregorian_destroy_date(greg_today);
    }
}

int main(int argc, char* argv[]) {
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning
    
    print_header();
    demonstrate_calendars();
    
    // Start interactive menu
    console_ui_run();
    
    return 0;
}