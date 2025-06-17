#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/calendars/gregorian.h"
#include "../src/utils/date_utils.h"

void test_gregorian_leap_year(void);
void test_gregorian_days_in_month(void);
void test_gregorian_day_of_week(void);
void test_gregorian_date_creation(void);
void test_gregorian_date_validation(void);
void test_gregorian_date_arithmetic(void);
void test_gregorian_julian_conversion(void);
void test_gregorian_printing(void);

void run_gregorian_tests(void) {
    printf("Running Gregorian Calendar Tests...\n");
    
    test_gregorian_leap_year();
    test_gregorian_days_in_month();
    test_gregorian_day_of_week();
    test_gregorian_date_creation();
    test_gregorian_date_validation();
    test_gregorian_date_arithmetic();
    test_gregorian_julian_conversion();
    test_gregorian_printing();
    
    printf("✅ All Gregorian calendar tests passed!\n\n");
}

void test_gregorian_leap_year(void) {
    printf("  Testing leap year calculation...\n");
    
    // Test typical leap years
    assert(gregorian_is_leap_year(2000) == 1);  // Divisible by 400
    assert(gregorian_is_leap_year(2004) == 1);  // Divisible by 4
    assert(gregorian_is_leap_year(2020) == 1);  // Divisible by 4
    
    // Test non-leap years
    assert(gregorian_is_leap_year(1900) == 0);  // Divisible by 100 but not 400
    assert(gregorian_is_leap_year(2001) == 0);  // Not divisible by 4
    assert(gregorian_is_leap_year(2100) == 0);  // Divisible by 100 but not 400
    
    // Test edge cases
    assert(gregorian_is_leap_year(4) == 1);     // Early leap year
    assert(gregorian_is_leap_year(1) == 0);     // Year 1 is not leap
    
    printf("    ✓ Leap year tests passed\n");
}

void test_gregorian_days_in_month(void) {
    printf("  Testing days in month calculation...\n");
    
    // Test regular months in non-leap year
    assert(gregorian_days_in_month(1, 2021) == 31);   // January
    assert(gregorian_days_in_month(2, 2021) == 28);   // February (non-leap)
    assert(gregorian_days_in_month(3, 2021) == 31);   // March
    assert(gregorian_days_in_month(4, 2021) == 30);   // April
    assert(gregorian_days_in_month(5, 2021) == 31);   // May
    assert(gregorian_days_in_month(6, 2021) == 30);   // June
    assert(gregorian_days_in_month(7, 2021) == 31);   // July
    assert(gregorian_days_in_month(8, 2021) == 31);   // August
    assert(gregorian_days_in_month(9, 2021) == 30);   // September
    assert(gregorian_days_in_month(10, 2021) == 31);  // October
    assert(gregorian_days_in_month(11, 2021) == 30);  // November
    assert(gregorian_days_in_month(12, 2021) == 31);  // December
    
    // Test February in leap year
    assert(gregorian_days_in_month(2, 2020) == 29);   // February (leap year)
    assert(gregorian_days_in_month(2, 2000) == 29);   // February (leap year)
    
    // Test invalid months
    assert(gregorian_days_in_month(0, 2021) == -1);   // Invalid month
    assert(gregorian_days_in_month(13, 2021) == -1);  // Invalid month
    
    printf("    ✓ Days in month tests passed\n");
}

void test_gregorian_day_of_week(void) {
    printf("  Testing day of week calculation...\n");
    
    // Test known dates
    assert(gregorian_day_of_week(1, 1, 2000) == 6);   // Saturday
    assert(gregorian_day_of_week(1, 1, 2001) == 1);   // Monday
    assert(gregorian_day_of_week(25, 12, 2023) == 1); // Monday (Christmas 2023)
    assert(gregorian_day_of_week(4, 7, 1776) == 4);   // Thursday (US Independence)
    
    // Test edge cases
    assert(gregorian_day_of_week(29, 2, 2020) >= 0 && gregorian_day_of_week(29, 2, 2020) <= 6);
    
    printf("    ✓ Day of week tests passed\n");
}

void test_gregorian_date_creation(void) {
    printf("  Testing date creation and destruction...\n");
    
    // Test valid date creation
    GregorianDate* date = gregorian_create_date(25, 12, 2023);
    assert(date != NULL);
    assert(date->base.day == 25);
    assert(date->base.month == 12);
    assert(date->base.year == 2023);
    assert(date->day_of_week >= 0 && date->day_of_week <= 6);
    assert(date->julian_day > 0);
    
    // Test destruction
    gregorian_destroy_date(date);
    
    // Test invalid date creation
    date = gregorian_create_date(32, 1, 2023);  // Invalid day
    assert(date == NULL);
    
    date = gregorian_create_date(29, 2, 2021);  // Invalid leap day
    assert(date == NULL);
    
    date = gregorian_create_date(15, 13, 2023); // Invalid month
    assert(date == NULL);
    
    date = gregorian_create_date(15, 6, 0);     // Invalid year
    assert(date == NULL);
    
    printf("    ✓ Date creation tests passed\n");
}

void test_gregorian_date_validation(void) {
    printf("  Testing date validation...\n");
    
    // Test valid dates
    assert(gregorian_validate_date(1, 1, 2023) == CALENDAR_SUCCESS);
    assert(gregorian_validate_date(29, 2, 2020) == CALENDAR_SUCCESS);  // Leap day
    assert(gregorian_validate_date(31, 12, 2023) == CALENDAR_SUCCESS);
    
    // Test invalid dates
    assert(gregorian_validate_date(0, 1, 2023) == CALENDAR_ERROR_INVALID_DATE);     // Day too small
    assert(gregorian_validate_date(32, 1, 2023) == CALENDAR_ERROR_INVALID_DATE);    // Day too large
    assert(gregorian_validate_date(29, 2, 2021) == CALENDAR_ERROR_INVALID_DATE);    // Invalid leap day
    assert(gregorian_validate_date(15, 0, 2023) == CALENDAR_ERROR_INVALID_MONTH);   // Month too small
    assert(gregorian_validate_date(15, 13, 2023) == CALENDAR_ERROR_INVALID_MONTH);  // Month too large
    assert(gregorian_validate_date(15, 6, 0) == CALENDAR_ERROR_INVALID_YEAR);       // Invalid year
    
    printf("    ✓ Date validation tests passed\n");
}

void test_gregorian_date_arithmetic(void) {
    printf("  Testing date arithmetic...\n");
    
    // Test adding days
    GregorianDate* date1 = gregorian_create_date(25, 12, 2023);
    assert(date1 != NULL);
    
    GregorianDate* date2 = gregorian_add_days(date1, 7);  // Add one week
    assert(date2 != NULL);
    assert(date2->base.day == 1);
    assert(date2->base.month == 1);
    assert(date2->base.year == 2024);
    
    // Test days between dates
    int days_diff = gregorian_days_between(date1, date2);
    assert(days_diff == 7);
    
    // Test adding negative days
    GregorianDate* date3 = gregorian_add_days(date2, -7);
    assert(date3 != NULL);
    assert(date3->base.day == date1->base.day);
    assert(date3->base.month == date1->base.month);
    assert(date3->base.year == date1->base.year);
    
    // Test adding many days (cross year boundary)
    GregorianDate* date4 = gregorian_add_days(date1, 365);
    assert(date4 != NULL);
    assert(date4->base.year == 2024);  // Should be next year
    
    gregorian_destroy_date(date1);
    gregorian_destroy_date(date2);
    gregorian_destroy_date(date3);
    gregorian_destroy_date(date4);
    
    printf("    ✓ Date arithmetic tests passed\n");
}

void test_gregorian_julian_conversion(void) {
    printf("  Testing Julian day conversion...\n");
    
    // Test round-trip conversion
    GregorianDate* original = gregorian_create_date(15, 6, 2023);
    assert(original != NULL);
    
    long julian = original->julian_day;
    
    GregorianDate converted;
    CalendarResult result = gregorian_from_julian_day(julian, &converted);
    assert(result == CALENDAR_SUCCESS);
    assert(converted.base.day == original->base.day);
    assert(converted.base.month == original->base.month);
    assert(converted.base.year == original->base.year);
    
    // Test known Julian day numbers
    long jdn_2000 = gregorian_to_julian_day(1, 1, 2000);
    assert(jdn_2000 == 2451545);  // Known value for Jan 1, 2000
    
    gregorian_destroy_date(original);
    
    printf("    ✓ Julian day conversion tests passed\n");
}

void test_gregorian_printing(void) {
    printf("  Testing date printing (output not validated automatically)...\n");
    
    GregorianDate* date = gregorian_create_date(25, 12, 2023);
    if (date) {
        printf("    Sample date output: ");
        gregorian_print_date(date);
        gregorian_destroy_date(date);
    }
    
    printf("    Sample month calendar (December 2023):\n");
    gregorian_print_month(12, 2023);
    
    printf("    ✓ Printing tests completed\n");
}

// Main function for standalone testing
#ifdef TEST_GREGORIAN_STANDALONE
int main(void) {
    run_gregorian_tests();
    return 0;
}
#endif