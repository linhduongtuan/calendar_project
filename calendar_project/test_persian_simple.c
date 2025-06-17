// Simple test for Persian calendar functionality
#include <stdio.h>
#include <stdlib.h>
#include "src/calendars/persian.h"
#include "src/calendars/gregorian.h"

int main() {
    printf("🧪 Testing Persian Calendar Implementation\n");
    printf("==========================================\n\n");
    
    // Test 1: Create a Persian date
    printf("Test 1: Creating Persian New Year (Nowruz) 1404\n");
    PersianDate* nowruz = persian_create_date(1, 1, 1404);
    if (nowruz) {
        printf("✅ Persian date created successfully: ");
        persian_print_date(nowruz);
        printf("   Julian Day: %ld\n", nowruz->julian_day);
        printf("   Season: %s\n", nowruz->season);
        printf("   Is leap year: %s\n\n", nowruz->is_leap_year ? "Yes" : "No");
    } else {
        printf("❌ Failed to create Persian date\n\n");
        return 1;
    }
    
    // Test 2: Convert to Gregorian
    printf("Test 2: Converting to Gregorian calendar\n");
    GregorianDate greg_result;
    CalendarResult result = persian_to_gregorian(nowruz, &greg_result);
    if (result == CALENDAR_SUCCESS) {
        printf("✅ Conversion successful: ");
        gregorian_print_date(&greg_result);
        printf("   Expected: Around March 20-21, 2025\n\n");
    } else {
        printf("❌ Conversion failed\n\n");
    }
    
    // Test 3: Test leap year calculation
    printf("Test 3: Testing leap year calculations\n");
    int test_years[] = {1403, 1404, 1407, 1408, 1412};
    for (int i = 0; i < 5; i++) {
        int year = test_years[i];
        int is_leap = persian_is_leap_year(year);
        printf("   Year %d: %s\n", year, is_leap ? "Leap year" : "Regular year");
    }
    printf("\n");
    
    // Test 4: Test month lengths
    printf("Test 4: Testing month lengths for year 1404\n");
    for (int month = 1; month <= 12; month++) {
        int days = persian_days_in_month(month, 1404);
        printf("   %s: %d days\n", persian_months[month-1], days);
    }
    printf("\n");
    
    // Test 5: Test current date conversion
    printf("Test 5: Converting current Gregorian date to Persian\n");
    GregorianDate* today = gregorian_create_date(17, 6, 2025);
    if (today) {
        PersianDate persian_today;
        result = persian_from_gregorian(today, &persian_today);
        if (result == CALENDAR_SUCCESS) {
            printf("✅ Today in Gregorian: ");
            gregorian_print_date(today);
            printf("✅ Today in Persian: ");
            persian_print_date(&persian_today);
        } else {
            printf("❌ Failed to convert current date\n");
        }
        gregorian_destroy_date(today);
    }
    
    // Cleanup
    persian_destroy_date(nowruz);
    
    printf("\n🎉 Persian calendar test completed!\n");
    return 0;
}