#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/calendars/islamic.h"
#include "../src/calendars/gregorian.h"
#include "../src/utils/date_utils.h"

void test_islamic_leap_year(void);
void test_islamic_days_in_month(void);
void test_islamic_date_creation(void);
void test_islamic_date_validation(void);
void test_islamic_gregorian_conversion(void);
void test_islamic_printing(void);

void run_islamic_tests(void) {
    printf("Running Islamic Calendar Tests...\n");
    
    test_islamic_leap_year();
    test_islamic_days_in_month();
    test_islamic_date_creation();
    test_islamic_date_validation();
    test_islamic_gregorian_conversion();
    test_islamic_printing();
    
    printf("✅ All Islamic calendar tests passed!\n\n");
}

void test_islamic_leap_year(void) {
    printf("  Testing Islamic leap year calculation...\n");
    
    // Test known leap years in 30-year cycle
    assert(islamic_is_leap_year(2) == 1);   // Leap year
    assert(islamic_is_leap_year(5) == 1);   // Leap year
    assert(islamic_is_leap_year(7) == 1);   // Leap year
    assert(islamic_is_leap_year(10) == 1);  // Leap year
    assert(islamic_is_leap_year(13) == 1);  // Leap year
    assert(islamic_is_leap_year(16) == 1);  // Leap year
    assert(islamic_is_leap_year(18) == 1);  // Leap year
    assert(islamic_is_leap_year(21) == 1);  // Leap year
    assert(islamic_is_leap_year(24) == 1);  // Leap year
    assert(islamic_is_leap_year(26) == 1);  // Leap year
    assert(islamic_is_leap_year(29) == 1);  // Leap year
    
    // Test non-leap years
    assert(islamic_is_leap_year(1) == 0);   // Not leap year
    assert(islamic_is_leap_year(3) == 0);   // Not leap year
    assert(islamic_is_leap_year(4) == 0);   // Not leap year
    assert(islamic_is_leap_year(6) == 0);   // Not leap year
    assert(islamic_is_leap_year(8) == 0);   // Not leap year
    assert(islamic_is_leap_year(9) == 0);   // Not leap year
    assert(islamic_is_leap_year(11) == 0);  // Not leap year
    assert(islamic_is_leap_year(12) == 0);  // Not leap year
    assert(islamic_is_leap_year(14) == 0);  // Not leap year
    assert(islamic_is_leap_year(15) == 0);  // Not leap year
    
    // Test cycle repetition (year 32 should be same as year 2)
    assert(islamic_is_leap_year(32) == islamic_is_leap_year(2));
    assert(islamic_is_leap_year(62) == islamic_is_leap_year(2));
    
    printf("    ✓ Islamic leap year tests passed\n");
}

void test_islamic_days_in_month(void) {
    printf("  Testing Islamic days in month calculation...\n");
    
    // Test odd months (should have 30 days)
    assert(islamic_days_in_month(1, 1445) == 30);   // Muharram
    assert(islamic_days_in_month(3, 1445) == 30);   // Rabi' al-awwal
    assert(islamic_days_in_month(5, 1445) == 30);   // Jumada al-awwal
    assert(islamic_days_in_month(7, 1445) == 30);   // Rajab
    assert(islamic_days_in_month(9, 1445) == 30);   // Ramadan
    assert(islamic_days_in_month(11, 1445) == 30);  // Dhu al-Qi'dah
    
    // Test even months in non-leap year (should have 29 days)
    int non_leap_year = 1444;  // Assuming this is not a leap year
    if (!islamic_is_leap_year(non_leap_year)) {
        assert(islamic_days_in_month(2, non_leap_year) == 29);   // Safar
        assert(islamic_days_in_month(4, non_leap_year) == 29);   // Rabi' al-thani
        assert(islamic_days_in_month(6, non_leap_year) == 29);   // Jumada al-thani
        assert(islamic_days_in_month(8, non_leap_year) == 29);   // Sha'ban
        assert(islamic_days_in_month(10, non_leap_year) == 29);  // Shawwal
        assert(islamic_days_in_month(12, non_leap_year) == 29);  // Dhu al-Hijjah
    }
    
    // Test 12th month in leap year (should have 30 days)
    int leap_year = 1445;
    if (islamic_is_leap_year(leap_year)) {
        assert(islamic_days_in_month(12, leap_year) == 30);  // Dhu al-Hijjah in leap year
    }
    
    // Test invalid months
    assert(islamic_days_in_month(0, 1445) == -1);   // Invalid month
    assert(islamic_days_in_month(13, 1445) == -1);  // Invalid month
    
    printf("    ✓ Islamic days in month tests passed\n");
}

void test_islamic_date_creation(void) {
    printf("  Testing Islamic date creation and destruction...\n");
    
    // Test valid date creation
    IslamicDate* date = islamic_create_date(15, 6, 1445);
    assert(date != NULL);
    assert(date->base.day == 15);
    assert(date->base.month == 6);
    assert(date->base.year == 1445);
    assert(date->is_leap_year == islamic_is_leap_year(1445));
    assert(strlen(date->month_name) > 0);
    
    // Test destruction
    islamic_destroy_date(date);
    
    // Test invalid date creation
    date = islamic_create_date(31, 6, 1445);  // Invalid day (month 6 has max 29 days)
    assert(date == NULL);
    
    date = islamic_create_date(15, 13, 1445); // Invalid month
    assert(date == NULL);
    
    date = islamic_create_date(15, 6, 0);     // Invalid year
    assert(date == NULL);
    
    // Test date creation for 12th month in leap year
    if (islamic_is_leap_year(1445)) {
        date = islamic_create_date(30, 12, 1445);  // Should be valid in leap year
        assert(date != NULL);
        islamic_destroy_date(date);
    }
    
    printf("    ✓ Islamic date creation tests passed\n");
}

void test_islamic_date_validation(void) {
    printf("  Testing Islamic date validation...\n");
    
    // Test valid dates
    assert(islamic_validate_date(1, 1, 1445) == CALENDAR_SUCCESS);
    assert(islamic_validate_date(30, 1, 1445) == CALENDAR_SUCCESS);  // Odd month, 30 days
    assert(islamic_validate_date(29, 2, 1445) == CALENDAR_SUCCESS);  // Even month, 29 days
    
    // Test 12th month in leap year
    if (islamic_is_leap_year(1445)) {
        assert(islamic_validate_date(30, 12, 1445) == CALENDAR_SUCCESS);
    }
    
    // Test invalid dates
    assert(islamic_validate_date(0, 1, 1445) == CALENDAR_ERROR_INVALID_DATE);     // Day too small
    assert(islamic_validate_date(31, 1, 1445) == CALENDAR_ERROR_INVALID_DATE);    // Day too large
    assert(islamic_validate_date(30, 2, 1445) == CALENDAR_ERROR_INVALID_DATE);    // Even month, 30 days invalid
    assert(islamic_validate_date(15, 0, 1445) == CALENDAR_ERROR_INVALID_MONTH);   // Month too small
    assert(islamic_validate_date(15, 13, 1445) == CALENDAR_ERROR_INVALID_MONTH);  // Month too large
        assert(islamic_validate_date(15, 6, 0) == CALENDAR_ERROR_INVALID_YEAR);       // Invalid year
    
    printf("    ✓ Islamic date validation tests passed\n");
}

void test_islamic_gregorian_conversion(void) {
    printf("  Testing Islamic-Gregorian conversion...\n");
    
    // Test conversion from Gregorian to Islamic
    GregorianDate* greg_date = gregorian_create_date(1, 1, 2024);
    assert(greg_date != NULL);
    
    IslamicDate islamic_date;
    CalendarResult result = islamic_from_gregorian(greg_date, &islamic_date);
    assert(result == CALENDAR_SUCCESS);
    assert(islamic_date.base.year > 1440);  // Should be in 1440s AH
    
    // Test round-trip conversion
    GregorianDate converted_back;
    result = islamic_to_gregorian(&islamic_date, &converted_back);
    assert(result == CALENDAR_SUCCESS);
    
    // The conversion may not be exact due to approximation, but should be close
    int day_diff = abs(converted_back.base.day - greg_date->base.day);
    assert(day_diff <= 2);  // Allow small difference due to approximation
    
    gregorian_destroy_date(greg_date);
    
    // Test known historical date (approximate)
    greg_date = gregorian_create_date(16, 7, 622);  // Approximate start of Islamic calendar
    if (greg_date) {
        result = islamic_from_gregorian(greg_date, &islamic_date);
        if (result == CALENDAR_SUCCESS) {
            assert(islamic_date.base.year == 1);  // Should be year 1 AH
        }
        gregorian_destroy_date(greg_date);
    }
    
    printf("    ✓ Islamic-Gregorian conversion tests passed\n");
}

void test_islamic_printing(void) {
    printf("  Testing Islamic date printing (output not validated automatically)...\n");
    
    IslamicDate* date = islamic_create_date(15, 9, 1445);  // 15 Ramadan 1445
    if (date) {
        printf("    Sample Islamic date output: ");
        islamic_print_date(date);
        islamic_destroy_date(date);
    }
    
    printf("    Sample Islamic month calendar (Ramadan 1445):\n");
    islamic_print_month(9, 1445);
    
    printf("    ✓ Islamic printing tests completed\n");
}

// Main function for standalone testing
#ifdef TEST_ISLAMIC_STANDALONE
int main(void) {
    run_islamic_tests();
    return 0;
}
#endif