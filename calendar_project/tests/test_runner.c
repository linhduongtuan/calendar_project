#include <stdio.h>
#include <assert.h>
#include "../src/calendars/gregorian.h"
#include "../src/calendars/islamic.h"
#include "../src/calendars/chinese.h"
#include "../src/utils/date_utils.h"
#include "../src/utils/string_utils.h"

// Test function prototypes
void run_gregorian_tests(void);
void run_islamic_tests(void);
void run_chinese_tests(void);
void test_utility_functions(void);

int main() {
    printf("Running Calendar System Tests...\n\n");
    
    run_gregorian_tests();
    run_islamic_tests();
    run_chinese_tests();
    test_utility_functions();
    
    printf("\n✅ All tests passed!\n");
    return 0;
}

void run_gregorian_tests(void) {
    printf("Testing Gregorian Calendar Functions...\n");
    
    // Test leap year function
    assert(gregorian_is_leap_year(2000) == 1);  // Divisible by 400
    assert(gregorian_is_leap_year(1900) == 0);  // Divisible by 100 but not 400
    assert(gregorian_is_leap_year(2004) == 1);  // Divisible by 4
    assert(gregorian_is_leap_year(2001) == 0);  // Not divisible by 4
    printf("  ✓ Leap year tests passed\n");
    
    // Test days in month
    assert(gregorian_days_in_month(2, 2000) == 29);  // Leap year February
    assert(gregorian_days_in_month(2, 2001) == 28);  // Non-leap year February
    assert(gregorian_days_in_month(4, 2021) == 30);  // April
    assert(gregorian_days_in_month(1, 2021) == 31);  // January
    printf("  ✓ Days in month tests passed\n");
    
    // Test date creation and validation
    GregorianDate* date = gregorian_create_date(25, 12, 2023);
    assert(date != NULL);
    assert(date->base.day == 25);
    assert(date->base.month == 12);
    assert(date->base.year == 2023);
    gregorian_destroy_date(date);
    printf("  ✓ Date creation tests passed\n");
    
    // Test invalid date
    date = gregorian_create_date(32, 1, 2023);  // Invalid day
    assert(date == NULL);
    printf("  ✓ Invalid date handling tests passed\n");
    
    printf("✅ All Gregorian calendar tests passed!\n\n");
}

void run_islamic_tests(void) {
    printf("Testing Islamic Calendar Functions...\n");
    
    // Test leap year function
    assert(islamic_is_leap_year(2) == 1);   // Leap year in cycle
    assert(islamic_is_leap_year(1) == 0);   // Not a leap year
    printf("  ✓ Islamic leap year tests passed\n");
    
    // Test days in month
    assert(islamic_days_in_month(1, 1445) == 30);  // Odd month
    assert(islamic_days_in_month(2, 1445) == 29);  // Even month, non-leap year
    printf("  ✓ Islamic days in month tests passed\n");
    
    // Test date creation
    IslamicDate* date = islamic_create_date(15, 6, 1445);
    assert(date != NULL);
    assert(date->base.day == 15);
    assert(date->base.month == 6);
    assert(date->base.year == 1445);
    islamic_destroy_date(date);
    printf("  ✓ Islamic date creation tests passed\n");
    
    printf("✅ All Islamic calendar tests passed!\n\n");
}

void run_chinese_tests(void) {
    printf("Testing Chinese Calendar Functions...\n");
    
    // Test zodiac function
    const char* zodiac2024 = chinese_get_zodiac(2024);
    assert(zodiac2024 != NULL);
    assert(strlen(zodiac2024) > 0);
    printf("  ✓ Chinese zodiac tests passed\n");
    
    // Test stem-branch function
    char stem_branch[50];
    chinese_get_stem_branch(2024, stem_branch, sizeof(stem_branch));
    assert(strlen(stem_branch) > 0);
    printf("  ✓ Chinese stem-branch tests passed\n");
    
    // Test cycle year
    int cycle_year = chinese_get_cycle_year(2024);
    assert(cycle_year >= 1 && cycle_year <= 60);
    printf("  ✓ Chinese cycle year tests passed\n");
    
    // Test date creation
    ChineseDate* date = chinese_create_date(15, 6, 2024);
    assert(date != NULL);
    assert(date->base.day == 15);
    assert(date->base.month == 6);
    assert(date->base.year == 2024);
    chinese_destroy_date(date);
    printf("  ✓ Chinese date creation tests passed\n");
    
    printf("✅ All Chinese calendar tests passed!\n\n");
}

void test_utility_functions(void) {
    printf("Testing Utility Functions...\n");
    
    // Test modulo function
    assert(modulo(7, 3) == 1);
    assert(modulo(-2, 3) == 1);
    printf("  ✓ Modulo function tests passed\n");
    
    // Test validation functions
    assert(is_valid_day(15) == 1);
    assert(is_valid_day(0) == 0);
    assert(is_valid_day(32) == 0);
    
    assert(is_valid_month(6) == 1);
    assert(is_valid_month(0) == 0);
    assert(is_valid_month(13) == 0);
    
    assert(is_valid_year(2023) == 1);
    assert(is_valid_year(0) == 0);
    printf("  ✓ Validation function tests passed\n");
    
    // Test Julian day conversion
    long jdn = julian_day_from_date(1, 1, 2000);
    int day, month, year;
    julian_day_to_date(jdn, &day, &month, &year);
    assert(day == 1 && month == 1 && year == 2000);
    printf("  ✓ Julian day conversion tests passed\n");
    
    // Test string functions
    assert(string_is_empty("") == 1);
    assert(string_is_empty("  ") == 1);
    assert(string_is_empty("hello") == 0);
    printf("  ✓ String utility tests passed\n");
    
    printf("✅ All utility function tests passed!\n\n");
}