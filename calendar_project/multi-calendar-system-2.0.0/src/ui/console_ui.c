// src/ui/console_ui.c (Fixed version without undefined functions/fields)
#include "console_ui.h"
#include "../calendars/gregorian.h"
#include "../calendars/islamic.h"
#include "../calendars/chinese.h"
#include "../calendars/hebrew.h"
#include "../astronomy/lunar_calc.h"
#include "../holidays/holiday_db.h"
#include "../export/calendar_export.h"

static void show_gregorian_menu(void);
static void show_islamic_menu(void);
static void show_chinese_menu(void);
static void show_hebrew_menu(void);
static void show_conversion_menu(void);
static void show_date_calculator_menu(void);
static void show_export_menu(void);
static void show_astronomy_menu(void);

void console_ui_run(void) {
    int choice;
    
    do {
        printf("\n==================================================\n");
        printf("           MULTI-CALENDAR SYSTEM v2.0\n");
        printf("==================================================\n");
        printf("1. Gregorian Calendar\n");
        printf("2. Islamic Calendar\n");
        printf("3. Chinese Calendar\n");
        printf("4. Hebrew Calendar\n");
        printf("5. Calendar Conversions\n");
        printf("6. Date Calculator\n");
        printf("7. Export Calendars\n");
        printf("8. Astronomy & Moon Phases\n");
        printf("0. Exit\n");
        printf("==================================================\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1: show_gregorian_menu(); break;
            case 2: show_islamic_menu(); break;
            case 3: show_chinese_menu(); break;
            case 4: show_hebrew_menu(); break;
            case 5: show_conversion_menu(); break;
            case 6: show_date_calculator_menu(); break;
            case 7: show_export_menu(); break;
            case 8: show_astronomy_menu(); break;
            case 0: printf("Thank you for using Multi-Calendar System!\n"); break;
            default: printf("Invalid choice. Please try again.\n"); break;
        }
    } while (choice != 0);
}

static void show_gregorian_menu(void) {
    int month, year, submenu;
    
    printf("\n=== GREGORIAN CALENDAR ===\n");
    printf("1. Show current month\n");
    printf("2. Show specific month\n");
    printf("3. Show year overview\n");
    printf("4. Today's detailed information\n");
    printf("Enter choice: ");
    
    scanf("%d", &submenu);
    
    switch (submenu) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            gregorian_print_month(local_time->tm_mon + 1, local_time->tm_year + 1900);
            break;
        }
        case 2: {
            printf("Enter month (1-12): ");
            scanf("%d", &month);
            printf("Enter year: ");
            scanf("%d", &year);
            if (month >= 1 && month <= 12 && year > 0) {
                gregorian_print_month(month, year);
                
                // Show conversions for first day of month
                GregorianDate* greg_date = gregorian_create_date(1, month, year);
                if (greg_date) {
                    printf("\nFirst day conversions:\n");
                    
                    IslamicDate islamic_date;
                    if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                        printf("Islamic: %d %s %d AH\n", 
                               islamic_date.base.day, islamic_date.month_name, islamic_date.base.year);
                    }
                    
                    ChineseDate chinese_date;
                    if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                        printf("Chinese: Year of %s (%s)\n", 
                               chinese_date.zodiac, chinese_date.stem_branch);
                    }
                    
                    HebrewDate hebrew_date;
                    if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                        printf("Hebrew: %d %s %d\n", 
                               hebrew_date.base.day, hebrew_date.month_name, hebrew_date.base.year);
                    }
                    
                    gregorian_destroy_date(greg_date);
                }
            } else {
                printf("Invalid month or year.\n");
            }
            break;
        }
        case 3: {
            printf("Enter year: ");
            scanf("%d", &year);
            if (year > 0) {
                printf("\n=== YEAR %d OVERVIEW ===\n", year);
                for (int m = 1; m <= 12; m++) {
                    printf("\n%s %d:\n", gregorian_months[m-1], year);
                    gregorian_print_month(m, year);
                }
            }
            break;
        }
        case 4: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            if (today) {
                printf("\n=== TODAY'S DETAILED INFORMATION ===\n");
                gregorian_print_date(today);
                printf("Day of week: %s\n", gregorian_days[today->day_of_week]);
                printf("Julian Day: %ld\n", today->julian_day);
                
                // Calculate day of year manually
                int day_of_year = 0;
                for (int m = 1; m < today->base.month; m++) {
                    day_of_year += gregorian_days_in_month(m, today->base.year);
                }
                day_of_year += today->base.day;
                printf("Day of year: %d\n", day_of_year);
                
                // Moon phase
                double moon_age = lunar_calculate_moon_age(today->julian_day);
                LunarPhase phase = lunar_get_phase(moon_age);
                printf("Moon phase: %s (%.1f days old)\n", lunar_phase_name(phase), moon_age);
                
                gregorian_destroy_date(today);
            }
            break;
        }
    }
}

static void show_islamic_menu(void) {
    printf("\n=== ISLAMIC CALENDAR ===\n");
    printf("1. Current Islamic date\n");
    printf("2. Islamic month view\n");
    printf("3. Ramadan dates for year\n");
    printf("4. Prayer times (placeholder)\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            IslamicDate islamic_date;
            if (islamic_from_gregorian(today, &islamic_date) == CALENDAR_SUCCESS) {
                islamic_print_date(&islamic_date);
                printf("Month: %s\n", islamic_date.month_name);
                
                // Show moon phase (relevant for Islamic calendar)
                double moon_age = lunar_calculate_moon_age(today->julian_day);
                LunarPhase phase = lunar_get_phase(moon_age);
                printf("Moon phase: %s\n", lunar_phase_name(phase));
                printf("Moon age: %.1f days\n", moon_age);
            }
            
            gregorian_destroy_date(today);
            break;
        }
        case 2: {
            int month, year;
            printf("Enter Islamic month (1-12): ");
            scanf("%d", &month);
            printf("Enter Islamic year (AH): ");
            scanf("%d", &year);
            
            if (month >= 1 && month <= 12 && year > 0) {
                islamic_print_month(month, year);
            } else {
                printf("Invalid month or year.\n");
            }
            break;
        }
        case 3: {
            int year;
            printf("Enter Islamic year (AH): ");
            scanf("%d", &year);
            
            printf("\nRamadan %d AH:\n", year);
            printf("Month 9 (Ramadan) - approximately 29-30 days\n");
            printf("Begins: 1 Ramadan %d AH\n", year);
            printf("Ends: 29/30 Ramadan %d AH\n", year);
            printf("Followed by: Eid al-Fitr (1 Shawwal %d AH)\n", year);
            break;
        }
        case 4: {
            printf("Prayer times calculation not yet implemented.\n");
            printf("Would require geographic location and solar calculations.\n");
            break;
        }
    }
}

static void show_chinese_menu(void) {
    printf("\n=== CHINESE CALENDAR ===\n");
    printf("1. Current Chinese date\n");
    printf("2. Zodiac information\n");
    printf("3. Chinese New Year dates\n");
    printf("4. Traditional festivals\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            ChineseDate chinese_date;
            if (chinese_from_gregorian(today, &chinese_date) == CALENDAR_SUCCESS) {
                chinese_print_date(&chinese_date);
                printf("Zodiac: %s\n", chinese_date.zodiac);
                printf("Stem-Branch: %s\n", chinese_date.stem_branch);
                printf("Cycle Year: %d/60\n", chinese_date.cycle_year);
            }
            
            gregorian_destroy_date(today);
            break;
        }
        case 2: {
            int year;
            printf("Enter Gregorian year: ");
            scanf("%d", &year);
            
            GregorianDate* new_year = gregorian_create_date(1, 1, year);
            ChineseDate chinese_date;
            
            if (chinese_from_gregorian(new_year, &chinese_date) == CALENDAR_SUCCESS) {
                printf("\nChinese Zodiac for %d:\n", year);
                printf("Animal: %s\n", chinese_date.zodiac);
                printf("Stem-Branch: %s\n", chinese_date.stem_branch);
                printf("Characteristics: [Traditional zodiac traits for %s]\n", chinese_date.zodiac);
            }
            
            gregorian_destroy_date(new_year);
            break;
        }
        case 3: {
            printf("Chinese New Year dates (approximate):\n");
            printf("2024: February 10\n");
            printf("2025: January 29\n");
            printf("2026: February 17\n");
            printf("Note: Exact dates require lunar calendar calculations\n");
            break;
        }
        case 4: {
            printf("Traditional Chinese Festivals:\n");
            printf("Spring Festival (Chinese New Year)\n");
            printf("Lantern Festival (15th day of 1st lunar month)\n");
            printf("Dragon Boat Festival (5th day of 5th lunar month)\n");
            printf("Mid-Autumn Festival (15th day of 8th lunar month)\n");
            printf("Double Ninth Festival (9th day of 9th lunar month)\n");
            break;
        }
    }
}

static void show_hebrew_menu(void) {
    printf("\n=== HEBREW CALENDAR ===\n");
    printf("1. Current Hebrew date\n");
    printf("2. Hebrew month view\n");
    printf("3. Jewish holidays\n");
    printf("4. Sabbath times (placeholder)\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            HebrewDate hebrew_date;
            if (hebrew_from_gregorian(today, &hebrew_date) == CALENDAR_SUCCESS) {
                hebrew_print_date(&hebrew_date);
                printf("Month: %s\n", hebrew_date.month_name);
                printf("Year type: %s\n", hebrew_date.is_leap_year ? "Leap Year (13 months)" : "Regular Year (12 months)");
                
                if (hebrew_date.is_leap_year) {
                    printf("Leap month: Adar II\n");
                }
            }
            
            gregorian_destroy_date(today);
            break;
        }
        case 2: {
            int month, year;
            printf("Enter Hebrew month (1-12): ");
            scanf("%d", &month);
            printf("Enter Hebrew year: ");
            scanf("%d", &year);
            
            int max_months = hebrew_is_leap_year(year) ? 13 : 12;
            if (month >= 1 && month <= max_months && year > 0) {
                hebrew_print_month(month, year);
                printf("Days in month: %d\n", hebrew_days_in_month(month, year));
            } else {
                printf("Invalid month or year.\n");
            }
            break;
        }
        case 3: {
            printf("Major Jewish Holidays:\n");
            printf("Rosh Hashanah (1-2 Tishrei) - New Year\n");
            printf("Yom Kippur (10 Tishrei) - Day of Atonement\n");
            printf("Sukkot (15-21 Tishrei) - Festival of Booths\n");
            printf("Hanukkah (25 Kislev - 2/3 Tevet) - Festival of Lights\n");
            printf("Tu BiShvat (15 Shevat) - New Year of Trees\n");
            printf("Purim (14 Adar) - Festival of Lots\n");
            printf("Passover (15-22 Nisan) - Festival of Freedom\n");
            printf("Lag BaOmer (18 Iyar)\n");
            printf("Shavuot (6-7 Sivan) - Festival of Weeks\n");
            break;
        }
        case 4: {
            printf("Sabbath times calculation not yet implemented.\n");
            printf("Would require geographic location and solar calculations.\n");
            break;
        }
    }
}

static void show_conversion_menu(void) {
    printf("\n=== CALENDAR CONVERSIONS ===\n");
    printf("1. Gregorian to others\n");
    printf("2. Islamic to others\n");
    printf("3. Chinese to others\n");
    printf("4. Hebrew to others\n");
    printf("5. Julian Day conversions\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            int day, month, year;
            printf("Enter Gregorian date (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
            
            GregorianDate* greg_date = gregorian_create_date(day, month, year);
            if (greg_date) {
                printf("\nConversions for %d/%d/%d:\n", day, month, year);
                printf("Gregorian: %s, %s %d, %d (JD: %ld)\n",
                       gregorian_days[greg_date->day_of_week],
                       gregorian_months[month-1], day, year,
                       greg_date->julian_day);
                
                // Islamic
                IslamicDate islamic_date;
                if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                    printf("Islamic: %d %s %d AH\n",
                           islamic_date.base.day, islamic_date.month_name, islamic_date.base.year);
                }
                
                // Chinese
                ChineseDate chinese_date;
                if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                    printf("Chinese: Year of %s (%s), Cycle %d\n",
                           chinese_date.zodiac, chinese_date.stem_branch, chinese_date.cycle_year);
                }
                
                // Hebrew
                HebrewDate hebrew_date;
                if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                    printf("Hebrew: %d %s %d\n",
                           hebrew_date.base.day, hebrew_date.month_name, hebrew_date.base.year);
                }
                
                // Astronomy
                double moon_age = lunar_calculate_moon_age(greg_date->julian_day);
                LunarPhase phase = lunar_get_phase(moon_age);
                printf("Moon: %s (%.1f days old)\n", lunar_phase_name(phase), moon_age);
                
                gregorian_destroy_date(greg_date);
            } else {
                printf("Invalid date.\n");
            }
            break;
        }
        case 5: {
            long julian_day;
            printf("Enter Julian Day Number: ");
            scanf("%ld", &julian_day);
            
            int day, month, year;
            julian_day_to_date(julian_day, &day, &month, &year);
            
            printf("\nJulian Day %ld converts to:\n", julian_day);
            printf("Gregorian: %d/%d/%d\n", day, month, year);
            
            GregorianDate* greg_date = gregorian_create_date(day, month, year);
            if (greg_date) {
                // Show all conversions
                IslamicDate islamic_date;
                ChineseDate chinese_date;
                HebrewDate hebrew_date;
                
                if (islamic_from_gregorian(greg_date, &islamic_date) == CALENDAR_SUCCESS) {
                    printf("Islamic: %d %s %d AH\n",
                           islamic_date.base.day, islamic_date.month_name, islamic_date.base.year);
                }
                
                if (chinese_from_gregorian(greg_date, &chinese_date) == CALENDAR_SUCCESS) {
                    printf("Chinese: %s (%s)\n", chinese_date.zodiac, chinese_date.stem_branch);
                }
                
                if (hebrew_from_gregorian(greg_date, &hebrew_date) == CALENDAR_SUCCESS) {
                    printf("Hebrew: %d %s %d\n",
                           hebrew_date.base.day, hebrew_date.month_name, hebrew_date.base.year);
                }
                
                gregorian_destroy_date(greg_date);
            }
            break;
        }
        default:
            printf("Feature not yet implemented.\n");
            break;
    }
}

static void show_date_calculator_menu(void) {
    printf("\n=== DATE CALCULATOR ===\n");
    printf("1. Add days to date\n");
    printf("2. Subtract days from date\n");
    printf("3. Days between dates\n");
    printf("4. Day of week calculator\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            int day, month, year, days_to_add;
            printf("Enter date (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
            printf("Enter days to add: ");
            scanf("%d", &days_to_add);
            
            GregorianDate* date = gregorian_create_date(day, month, year);
            if (date) {
                long new_julian = date->julian_day + days_to_add;
                int new_day, new_month, new_year;
                julian_day_to_date(new_julian, &new_day, &new_month, &new_year);
                
                printf("Result: %d/%d/%d + %d days = %d/%d/%d\n",
                       day, month, year, days_to_add, new_day, new_month, new_year);
                
                gregorian_destroy_date(date);
            }
            break;
        }
        case 3: {
            int day1, month1, year1, day2, month2, year2;
            printf("Enter first date (DD MM YYYY): ");
            scanf("%d %d %d", &day1, &month1, &year1);
            printf("Enter second date (DD MM YYYY): ");
            scanf("%d %d %d", &day2, &month2, &year2);
            
            GregorianDate* date1 = gregorian_create_date(day1, month1, year1);
            GregorianDate* date2 = gregorian_create_date(day2, month2, year2);
            
            if (date1 && date2) {
                long difference = date2->julian_day - date1->julian_day;
                printf("Days between %d/%d/%d and %d/%d/%d: %ld days\n",
                       day1, month1, year1, day2, month2, year2, difference);
                
                if (difference < 0) {
                    printf("(Second date is earlier than first date)\n");
                }
            }
            
            if (date1) gregorian_destroy_date(date1);
            if (date2) gregorian_destroy_date(date2);
            break;
        }
        case 4: {
            int day, month, year;
            printf("Enter date (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
            
            GregorianDate* date = gregorian_create_date(day, month, year);
            if (date) {
                printf("%d/%d/%d falls on a %s\n",
                       day, month, year, gregorian_days[date->day_of_week]);
                gregorian_destroy_date(date);
            }
            break;
        }
        default:
            printf("Feature not yet implemented.\n");
            break;
    }
}

static void show_export_menu(void) {
    printf("\n=== EXPORT CALENDARS ===\n");
    printf("1. Export current month to CSV\n");
    printf("2. Export year to CSV\n");
    printf("3. Export to iCal format\n");
    printf("4. Export to JSON\n");
    printf("5. Export with holidays\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            ExportOptions options = {0};
            options.format = EXPORT_CSV;
            options.calendar_type = CALENDAR_GREGORIAN;
            options.start_year = local_time->tm_year + 1900;
            options.end_year = local_time->tm_year + 1900;
            options.include_holidays = 1;
            options.include_moon_phases = 1;
            strcpy(options.country_code, "US");
            strcpy(options.filename, "current_month.csv");
            
            CalendarResult result = export_calendar_month(&options, 
                                                        local_time->tm_mon + 1, 
                                                        local_time->tm_year + 1900);
            if (result == CALENDAR_SUCCESS) {
                printf("Successfully exported current month to current_month.csv\n");
            } else {
                printf("Export failed.\n");
            }
            break;
        }
        default: {
            printf("Feature not yet fully implemented.\n");
            break;
        }
    }
}

static void show_astronomy_menu(void) {
    printf("\n=== ASTRONOMY & MOON PHASES ===\n");
    printf("1. Current moon phase\n");
    printf("2. Moon phases for month\n");
    printf("3. Next new moon\n");
    printf("4. Next full moon\n");
    printf("5. Precise lunar calculations\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            if (today) {
                double moon_age = lunar_calculate_moon_age(today->julian_day);
                LunarPhase phase = lunar_get_phase(moon_age);
                double illumination = lunar_calculate_illumination(moon_age);
                
                printf("\nCurrent Moon Information:\n");
                printf("Phase: %s\n", lunar_phase_name(phase));
                printf("Age: %.2f days\n", moon_age);
                printf("Illumination: %.1f%%\n", illumination * 100);
                
                // Precise calculations
                PreciseLunarInfo* precise = lunar_calculate_precise(today->julian_day);
                if (precise) {
                    printf("Distance: %.0f km\n", precise->moon_distance_km);
                    printf("Phase angle: %.1f degrees\n", precise->moon_phase_angle);
                    free(precise);
                }
                
                gregorian_destroy_date(today);
            }
            break;
        }
        case 3: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            if (today) {
                double next_new = lunar_next_new_moon(today->julian_day);
                
                int day, month, year;
                julian_day_to_date((long)next_new, &day, &month, &year);
                
                printf("Next new moon: %d/%d/%d\n", day, month, year);
                printf("Julian Day: %.1f\n", next_new);
                
                gregorian_destroy_date(today);
            }
            break;
        }
        case 4: {
            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            
            GregorianDate* today = gregorian_create_date(
                local_time->tm_mday, 
                local_time->tm_mon + 1, 
                local_time->tm_year + 1900
            );
            
            if (today) {
                double next_full = lunar_next_full_moon(today->julian_day);
                
                int day, month, year;
                julian_day_to_date((long)next_full, &day, &month, &year);
                
                printf("Next full moon: %d/%d/%d\n", day, month, year);
                printf("Julian Day: %.1f\n", next_full);
                
                gregorian_destroy_date(today);
            }
            break;
        }
        case 5: {
            int day, month, year;
            printf("Enter date for lunar calculations (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
            
            GregorianDate* date = gregorian_create_date(day, month, year);
            if (date) {
                PreciseLunarInfo* info = lunar_calculate_precise(date->julian_day);
                if (info) {
                    printf("\nPrecise Lunar Information for %d/%d/%d:\n", day, month, year);
                    printf("Phase: %s\n", lunar_phase_name(info->phase));
                    printf("Moon age: %.3f days\n", info->moon_age_precise);
                    printf("Phase angle: %.2f degrees\n", info->moon_phase_angle);
                    printf("Illumination: %.2f%%\n", info->moon_illumination * 100);
                    printf("Distance: %.1f km\n", info->moon_distance_km);
                    printf("Julian Day: %.5f\n", info->julian_day);
                    
                    free(info);
                }
                gregorian_destroy_date(date);
            }
            break;
        }
        default:
            printf("Invalid choice.\n");
            break;
    }
}