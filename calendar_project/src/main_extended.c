// src/main_extended.c - Enhanced Multi-Calendar System with New Features
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Include all calendar systems
#include "calendars/gregorian.h"
#include "calendars/islamic.h"
#include "calendars/chinese.h"
#include "calendars/hebrew.h"
#include "calendars/persian.h"
#include "calendars/calendar_registry.h"

// Include enhanced features
#include "astronomy/solar_calc.h"
#include "astronomy/lunar_calc.h"
#include "religious/prayer_times.h"
#include "holidays/holiday_db.h"
#include "ui/console_ui.h"
#include "utils/date_utils.h"

void print_enhanced_header() {
    time_t current_time;
    struct tm *utc_time;
    
    time(&current_time);
    utc_time = gmtime(&current_time);
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║              🗓️  MULTI-CALENDAR SYSTEM v2.1.0               ║\n");
    printf("║                    Enhanced Edition                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ Current UTC Time: %04d-%02d-%02d %02d:%02d:%02d                ║\n",
           utc_time->tm_year + 1900, utc_time->tm_mon + 1, utc_time->tm_mday,
           utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
    printf("║ User: linhduongtuan                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");
}

void demonstrate_calendar_systems(CalendarRegistry* registry) {
    printf("📅 SUPPORTED CALENDAR SYSTEMS:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    // Get current date
    time_t now = time(NULL);
    struct tm *current = localtime(&now);
    
    GregorianDate* greg_today = gregorian_create_date(
        current->tm_mday,
        current->tm_mon + 1,
        current->tm_year + 1900
    );
    
    if (!greg_today) {
        printf("Error: Could not create current date\n");
        return;
    }
    
    printf("Today's Date in Multiple Calendar Systems:\n\n");
    
    // Gregorian
    printf("🌍 Gregorian: ");
    gregorian_print_date(greg_today);
    
    // Islamic
    IslamicDate islamic_today;
    if (islamic_from_gregorian(greg_today, &islamic_today) == CALENDAR_SUCCESS) {
        printf("🌙 Islamic:   ");
        islamic_print_date(&islamic_today);
    }
    
    // Persian
    PersianDate persian_today;
    if (persian_from_gregorian(greg_today, &persian_today) == CALENDAR_SUCCESS) {
        printf("☀️  Persian:   ");
        persian_print_date(&persian_today);
    }
    
    // Chinese
    ChineseDate chinese_today;
    if (chinese_from_gregorian(greg_today, &chinese_today) == CALENDAR_SUCCESS) {
        printf("🐉 Chinese:   ");
        chinese_print_date(&chinese_today);
    }
    
    // Hebrew
    HebrewDate hebrew_today;
    if (hebrew_from_gregorian(greg_today, &hebrew_today) == CALENDAR_SUCCESS) {
        printf("✡️  Hebrew:    ");
        hebrew_print_date(&hebrew_today);
    } else {
        printf("✡️  Hebrew:    Conversion not available\n");
    }
    
    printf("\n");
    gregorian_destroy_date(greg_today);
}

void demonstrate_astronomical_features() {
    printf("🌟 ASTRONOMICAL INFORMATION:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    // Get current Julian Day
    time_t now = time(NULL);
    struct tm *current = gmtime(&now);
    GregorianDate* today = gregorian_create_date(
        current->tm_mday,
        current->tm_mon + 1,
        current->tm_year + 1900
    );
    
    if (!today) return;
    
    double julian_day = (double)today->julian_day;
    
    // Lunar information
    double moon_age = lunar_calculate_moon_age(today->julian_day);
    LunarPhase phase = lunar_get_phase(moon_age);
    double illumination = lunar_calculate_illumination(moon_age);
    
    printf("🌙 Lunar Information:\n");
    printf("   Phase:        %s\n", lunar_phase_name(phase));
    printf("   Age:          %.1f days\n", moon_age);
    printf("   Illumination: %.1f%%\n", illumination * 100);
    
    // Solar information (if solar_calc is implemented)
    SolarPosition* solar_pos = solar_calculate_position(julian_day);
    if (solar_pos) {
        printf("\n☀️  Solar Information:\n");
        printf("   Solar Longitude: %.2f°\n", solar_pos->solar_longitude);
        printf("   Declination:     %.2f°\n", solar_pos->declination);
        printf("   Equation of Time: %.1f minutes\n", solar_pos->equation_of_time);
        printf("   Distance:        %.6f AU\n", solar_pos->solar_distance);
        solar_destroy_position(solar_pos);
    }
    
    // Seasonal information
    SeasonalEvents* seasons = solar_calculate_seasons(today->base.year);
    if (seasons) {
        printf("\n🍂 Seasonal Events for %d:\n", today->base.year);
        printf("   Spring Equinox: JD %.1f\n", seasons->spring_equinox_jd);
        printf("   Summer Solstice: JD %.1f\n", seasons->summer_solstice_jd);
        printf("   Autumn Equinox: JD %.1f\n", seasons->autumn_equinox_jd);
        printf("   Winter Solstice: JD %.1f\n", seasons->winter_solstice_jd);
        solar_destroy_seasons(seasons);
    }
    
    printf("\n");
    gregorian_destroy_date(today);
}

void demonstrate_prayer_times() {
    printf("🕌 ISLAMIC PRAYER TIMES:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    // Example coordinates (Mecca, Saudi Arabia)
    double latitude = 21.4225;
    double longitude = 39.8262;
    int timezone_offset = 3; // UTC+3
    
    time_t now = time(NULL);
    struct tm *current = gmtime(&now);
    GregorianDate* today = gregorian_create_date(
        current->tm_mday,
        current->tm_mon + 1,
        current->tm_year + 1900
    );
    
    if (!today) return;
    
    printf("Prayer Times for Mecca (%.4f°N, %.4f°E) - UTC+%d:\n\n", 
           latitude, longitude, timezone_offset);
    
    // Calculate prayer times using different methods
    PrayerCalculationMethod methods[] = {
        PRAYER_METHOD_MWL,
        PRAYER_METHOD_MAKKAH,
        PRAYER_METHOD_ISNA
    };
    
    const char* method_names[] = {
        "Muslim World League",
        "Umm Al-Qura (Makkah)",
        "ISNA"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("📿 %s Method:\n", method_names[i]);
        
        PrayerTimes* times = prayer_calculate_times(
            (double)today->julian_day,
            latitude, longitude,
            methods[i], timezone_offset
        );
        
        if (times) {
            prayer_print_times_12hour(times);
            prayer_destroy_times(times);
        }
        printf("\n");
    }
    
    // Qibla direction
    QiblaDirection* qibla = prayer_calculate_qibla(latitude, longitude);
    if (qibla) {
        printf("🧭 Qibla Direction:\n");
        prayer_print_qibla(qibla);
        prayer_destroy_qibla(qibla);
    }
    
    printf("\n");
    gregorian_destroy_date(today);
}

void demonstrate_holiday_database() {
    printf("🎉 CULTURAL HOLIDAYS & CELEBRATIONS:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    
    // Create and load holiday database
    HolidayDatabase* db = holiday_db_create();
    if (!db) {
        printf("Error: Could not create holiday database\n");
        return;
    }
    
    // Try to load from file (if exists)
    CalendarResult result = holiday_db_load_from_file(db, "data/holidays.json");
    if (result != CALENDAR_SUCCESS) {
        printf("Note: Holiday database file not found, using built-in holidays\n\n");
        
        // Add some sample holidays manually
        Holiday sample_holidays[] = {
            {"New Year's Day", "Beginning of Gregorian calendar year", 
             CALENDAR_GREGORIAN, HOLIDAY_FIXED, {1, 1, 2025}, "US", 1},
            {"Eid al-Fitr", "End of Ramadan celebration", 
             CALENDAR_ISLAMIC, HOLIDAY_LUNAR, {1, 10, 1446}, "SA", 1},
            {"Nowruz", "Persian New Year", 
             CALENDAR_PERSIAN, HOLIDAY_FIXED, {1, 1, 1404}, "IR", 1},
            {"Chinese New Year", "Beginning of Chinese lunar year", 
             CALENDAR_CHINESE, HOLIDAY_LUNAR, {1, 1, 4723}, "CN", 1}
        };
        
        for (int i = 0; i < 4; i++) {
            holiday_db_add_holiday(db, &sample_holidays[i]);
        }
    }
    
    printf("Upcoming Holidays (Sample):\n");
    holiday_db_print_holidays(db);
    
    holiday_db_destroy(db);
    printf("\n");
}

void show_enhanced_menu() {
    printf("🎯 ENHANCED FEATURES MENU:\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("1.  📅 Calendar Conversions\n");
    printf("2.  🌙 Lunar Calculations\n");
    printf("3.  ☀️  Solar Calculations\n");
    printf("4.  🕌 Prayer Times Calculator\n");
    printf("5.  🧭 Qibla Direction\n");
    printf("6.  🎉 Holiday Database\n");
    printf("7.  🌍 Timezone Conversions\n");
    printf("8.  📊 Calendar Statistics\n");
    printf("9.  📁 Export Data\n");
    printf("10. ⚙️  Preferences\n");
    printf("11. ❓ Help & Documentation\n");
    printf("0.  🚪 Exit\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("Enter your choice (0-11): ");
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // Suppress unused parameter warnings
    
    print_enhanced_header();
    
    // Initialize calendar registry
    CalendarRegistry* registry = calendar_registry_create_default();
    if (!registry) {
        printf("Error: Could not initialize calendar registry\n");
        return 1;
    }
    
    // Register all standard calendar systems
    calendar_registry_register_all_standard_systems(registry);
    
    // Demonstrate features
    demonstrate_calendar_systems(registry);
    demonstrate_astronomical_features();
    demonstrate_prayer_times();
    demonstrate_holiday_database();
    
    // Interactive menu
    int choice;
    do {
        show_enhanced_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("\n🔄 Calendar Conversion Module\n");
                handle_conversion_menu();
                break;
            case 2:
                printf("\n🌙 Lunar Calculation Module\n");
                demonstrate_astronomical_features();
                break;
            case 3:
                printf("\n☀️ Solar Calculation Module\n");
                // Call solar calculation demo
                break;
            case 4:
                printf("\n🕌 Prayer Times Module\n");
                demonstrate_prayer_times();
                break;
            case 5:
                printf("\n🧭 Qibla Direction Module\n");
                // Call qibla calculation
                break;
            case 6:
                printf("\n🎉 Holiday Database Module\n");
                demonstrate_holiday_database();
                break;
            case 7:
                printf("\n🌍 Timezone Module\n");
                // Call timezone conversion
                break;
            case 8:
                printf("\n📊 Calendar Statistics Module\n");
                calendar_registry_list_systems(registry);
                break;
            case 9:
                printf("\n📁 Export Module\n");
                // Call export functionality
                break;
            case 10:
                printf("\n⚙️ Preferences Module\n");
                // Call preferences management
                break;
            case 11:
                printf("\n❓ Help & Documentation\n");
                printf("Visit: https://github.com/linhduongtuan/multi-calendar-system\n");
                break;
            case 0:
                printf("\n👋 Thank you for using Multi-Calendar System!\n");
                break;
            default:
                printf("\n❌ Invalid choice. Please try again.\n");
        }
        
        if (choice != 0) {
            printf("\nPress Enter to continue...");
            while (getchar() != '\n'); // Clear input buffer
            getchar(); // Wait for Enter
            printf("\n");
        }
        
    } while (choice != 0);
    
    // Cleanup
    calendar_registry_destroy(registry);
    
    return 0;
}