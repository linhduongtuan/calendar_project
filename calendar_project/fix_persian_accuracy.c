// Test program to find the correct Persian calendar epoch
#include <stdio.h>
#include "src/calendars/gregorian.h"

// Test known accurate conversions
int main() {
    printf("Finding correct Persian calendar epoch...\n");
    
    // Known accurate reference: March 21, 2025 = 1 Farvardin 1404
    GregorianDate* nowruz_2025 = gregorian_create_date(21, 3, 2025);
    if (nowruz_2025) {
        printf("Nowruz 2025 (March 21, 2025) = Julian Day %ld\n", nowruz_2025->julian_day);
        
        // Calculate what the epoch should be for 1 Farvardin 1404
        // If March 21, 2025 is 1 Farvardin 1404, then we need to calculate backwards
        long days_since_epoch = 0;
        
        // Add days for complete years (1 to 1403)
        for (int y = 1; y < 1404; y++) {
            // Simplified leap year check for now
            int is_leap = ((y - 1) % 33 == 0 || (y - 1) % 33 == 4 || (y - 1) % 33 == 8 || 
                          (y - 1) % 33 == 12 || (y - 1) % 33 == 16 || (y - 1) % 33 == 21 || 
                          (y - 1) % 33 == 25 || (y - 1) % 33 == 29);
            days_since_epoch += is_leap ? 366 : 365;
        }
        
        // 1 Farvardin is day 1, so subtract 1
        days_since_epoch += 0; // No additional days for 1 Farvardin
        
        long calculated_epoch = nowruz_2025->julian_day - days_since_epoch;
        printf("Calculated epoch should be: %ld\n", calculated_epoch);
        printf("Current epoch in code: 1948321\n");
        printf("Difference: %ld days\n", calculated_epoch - 1948321);
        
        gregorian_destroy_date(nowruz_2025);
    }
    
    return 0;
}