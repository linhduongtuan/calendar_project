#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/calendars/chinese.h"
#include "../src/calendars/gregorian.h"
#include "../src/calendars/lunar_base.h"
#include "../src/utils/date_utils.h"
#include "../src/utils/string_utils.h"

void test_chinese_zodiac(void);
void test_chinese_stem_branch(void);
void test_chinese_cycle_year(void);
void test_chinese_date_creation(void);
void test_chinese_gregorian_conversion(void);
void test_chinese_lunar_base_functions(void);
void test_chinese_printing(void);

void run_chinese_tests(void) {
    printf("Running Chinese Calendar Tests...\n");
    
    test_chinese_zodiac();
    test_chinese_stem_branch();
    test_chinese_cycle_year();
    test_chinese_date_creation();
    test_chinese_gregorian_conversion();
    test_chinese_lunar_base_functions();
    test_chinese_printing();
    
    printf("✅ All Chinese calendar tests passed!\n\n");
}

void test_chinese_zodiac(void) {
    printf("  Testing Chinese zodiac calculation...\n");
    
    // Test known zodiac years (based on 1924 = Rat)
    assert(strcmp(chinese_get_zodiac(1924), "Rat") == 0);
    assert(strcmp(chinese_get_zodiac(1925), "Ox") == 0);
    assert(strcmp(chinese_get_zodiac(1926), "Tiger") == 0);
    assert(strcmp(chinese_get_zodiac(1927), "Rabbit") == 0);
    assert(strcmp(chinese_get_zodiac(1928), "Dragon") == 0);
    assert(strcmp(chinese_get_zodiac(1929), "Snake") == 0);
    assert(strcmp(chinese_get_zodiac(1930), "Horse") == 0);
    assert(strcmp(chinese_get_zodiac(1931), "Goat") == 0);
    assert(strcmp(chinese_get_zodiac(1932), "Monkey") == 0);
    assert(strcmp(chinese_get_zodiac(1933), "Rooster") == 0);
    assert(strcmp(chinese_get_zodiac(1934), "Dog") == 0);
    assert(strcmp(chinese_get_zodiac(1935), "Pig") == 0);
    
    // Test cycle repetition (12-year cycle)
    assert(strcmp(chinese_get_zodiac(1936), "Rat") == 0);      // 1924 + 12
    assert(strcmp(chinese_get_zodiac(2024), "Dragon") == 0);   // Should be Dragon
    assert(strcmp(chinese_get_zodiac(2025), "Snake") == 0);    // Should be Snake
    
    // Test earlier years
    assert(strcmp(chinese_get_zodiac(1912), "Rat") == 0);      // 1924 - 12
    
    printf("    ✓ Chinese zodiac tests passed\n");
}

void test_chinese_stem_branch(void) {
    printf("  Testing Chinese stem-branch calculation...\n");
    
    char stem_branch[50];
    
    // Test known combinations
    chinese_get_stem_branch(1924, stem_branch, sizeof(stem_branch));
    assert(strstr(stem_branch, "Jia") != NULL);  // Should start with Jia
    assert(strstr(stem_branch, "Zi") != NULL);   // Should end with Zi
    
    chinese_get_stem_branch(1925, stem_branch, sizeof(stem_branch));
    assert(strstr(stem_branch, "Yi") != NULL);   // Should start with Yi
    assert(strstr(stem_branch, "Chou") != NULL); // Should end with Chou
    
    // Test that result is not empty and contains a dash
    chinese_get_stem_branch(2024, stem_branch, sizeof(stem_branch));
    assert(strlen(stem_branch) > 0);
    assert(strstr(stem_branch, "-") != NULL);
    
    // Test 60-year cycle repetition
    char stem_branch_1924[50], stem_branch_1984[50];
    chinese_get_stem_branch(1924, stem_branch_1924, sizeof(stem_branch_1924));
    chinese_get_stem_branch(1984, stem_branch_1984, sizeof(stem_branch_1984));  // 1924 + 60
    assert(strcmp(stem_branch_1924, stem_branch_1984) == 0);
    
    printf("    ✓ Chinese stem-branch tests passed\n");
}

void test_chinese_cycle_year(void) {
    printf("  Testing Chinese 60-year cycle calculation...\n");
    
    // Test that cycle year is in valid range
    for (int year = 1924; year <= 2024; year++) {
        int cycle_year = chinese_get_cycle_year(year);
        assert(cycle_year >= 1 && cycle_year <= 60);
    }
    
    // Test specific known values
    assert(chinese_get_cycle_year(1924) == 1);   // Start of cycle
    assert(chinese_get_cycle_year(1983) == 60);  // End of cycle
    assert(chinese_get_cycle_year(1984) == 1);   // Start of next cycle
    
    // Test current year
    int current_cycle = chinese_get_cycle_year(2024);
    assert(current_cycle >= 1 && current_cycle <= 60);
    
    printf("    ✓ Chinese cycle year tests passed\n");
}

void test_chinese_date_creation(void) {
    printf("  Testing Chinese date creation and destruction...\n");
    
    // Test valid date creation
    ChineseDate* date = chinese_create_date(15, 6, 2024);
    assert(date != NULL);
    assert(date->base.day == 15);
    assert(date->base.month == 6);
    assert(date->base.year == 2024);
    assert(strlen(date->zodiac) > 0);
    assert(strlen(date->stem_branch) > 0);
    assert(date->cycle_year >= 1 && date->cycle_year <= 60);
    
    // Test destruction
    chinese_destroy_date(date);
    
    // Test that zodiac and stem-branch are set correctly
    date = chinese_create_date(1, 1, 1924);
    if (date) {
        assert(strcmp(date->zodiac, "Rat") == 0);
        assert(date->cycle_year == 1);
        chinese_destroy_date(date);
    }
    
    printf("    ✓ Chinese date creation tests passed\n");
}

void test_chinese_gregorian_conversion(void) {
    printf("  Testing Chinese-Gregorian conversion...\n");
    
    // Test conversion from Gregorian to Chinese
    GregorianDate* greg_date = gregorian_create_date(1, 1, 2024);
    assert(greg_date != NULL);
    
    ChineseDate chinese_date;
    CalendarResult result = chinese_from_gregorian(greg_date, &chinese_date);
    assert(result == CALENDAR_SUCCESS);
    
    // Check that basic conversion works (simplified implementation)
    assert(chinese_date.base.year == 2024);  // Should maintain year in simplified version
    assert(strlen(chinese_date.zodiac) > 0);
    assert(strlen(chinese_date.stem_branch) > 0);
    
    gregorian_destroy_date(greg_date);
    
    // Test multiple years
    for (int year = 2020; year <= 2025; year++) {
        greg_date = gregorian_create_date(15, 6, year);
        if (greg_date) {
            result = chinese_from_gregorian(greg_date, &chinese_date);
            assert(result == CALENDAR_SUCCESS);
            assert(strlen(chinese_date.zodiac) > 0);
            gregorian_destroy_date(greg_date);
        }
    }
    
    printf("    ✓ Chinese-Gregorian conversion tests passed\n");
}

void test_chinese_lunar_base_functions(void) {
    printf("  Testing lunar base functions...\n");
    
    // Test lunar phase calculation
    long julian_day = julian_day_from_date(1, 1, 2024);
    double moon_age = lunar_calculate_moon_age(julian_day);
    assert(moon_age >= 0.0 && moon_age < SYNODIC_MONTH);
    
    LunarPhase phase = lunar_get_phase(moon_age);
    assert(phase >= LUNAR_NEW_MOON && phase <= LUNAR_WANING_CRESCENT);
    
    const char* phase_name = lunar_phase_name(phase);
    assert(phase_name != NULL);
    assert(strlen(phase_name) > 0);
    
    // Test new moon calculation
    double next_new_moon = lunar_next_new_moon(julian_day);
    assert(next_new_moon > julian_day);
    
    double prev_new_moon = lunar_previous_new_moon(julian_day);
    assert(prev_new_moon <= julian_day);
    
    // Test lunar month info
    LunarMonthInfo month_info;
    CalendarResult result = lunar_get_month_info(2024, 6, &month_info);
    assert(result == CALENDAR_SUCCESS);
    assert(month_info.year == 2024);
    assert(month_info.month == 6);
    assert(month_info.days == 29 || month_info.days == 30);
    
    // Test lunar year functions
    int has_leap = lunar_has_leap_month(2024);
    assert(has_leap == 0 || has_leap == 1);
    
    int days_in_year = lunar_days_in_year(2024);
    assert(days_in_year >= 354 && days_in_year <= 384);  // Normal range for lunar year
    
    // Test solar terms
    SolarTerm terms[24];
    result = lunar_get_solar_terms(2024, terms);
    assert(result == CALENDAR_SUCCESS);
    
    // Check that all terms are filled
    for (int i = 0; i < 24; i++) {
        assert(terms[i].month >= 1 && terms[i].month <= 12);
        assert(terms[i].day >= 1 && terms[i].day <= 31);
        assert(strlen(terms[i].name) > 0);
    }
    
    int current_term = lunar_get_current_solar_term(julian_day);
    assert(current_term >= 0 && current_term < 24);
    
    printf("    ✓ Lunar base function tests passed\n");
}

void test_chinese_printing(void) {
    printf("  Testing Chinese date printing (output not validated automatically)...\n");
    
    ChineseDate* date = chinese_create_date(15, 6, 2024);
    if (date) {
        printf("    Sample Chinese date output: ");
        chinese_print_date(date);
        chinese_destroy_date(date);
    }
    
    // Test different zodiac years
    printf("    Sample zodiac years:\n");
    for (int year = 2020; year <= 2025; year++) {
        printf("      %d: %s\n", year, chinese_get_zodiac(year));
    }
    
    printf("    ✓ Chinese printing tests completed\n");
}

// Main function for standalone testing
#ifdef TEST_CHINESE_STANDALONE
int main(void) {
    run_chinese_tests();
    return 0;
}
#endif