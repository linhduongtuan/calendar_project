#include "islamic.h"
#include "gregorian.h"
#include "../utils/date_utils.h"

// Islamic month names
const char* islamic_months[ISLAMIC_MONTHS_COUNT] = {
    "Muharram", "Safar", "Rabi' al-awwal", "Rabi' al-thani",
    "Jumada al-awwal", "Jumada al-thani", "Rajab", "Sha'ban",
    "Ramadan", "Shawwal", "Dhu al-Qi'dah", "Dhu al-Hijjah"
};

int islamic_is_leap_year(int year) {
    // Islamic leap year cycle: years 2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29 in a 30-year cycle
    int cycle_year = year % 30;
    int leap_years[] = {2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29};
    
    for (int i = 0; i < 11; i++) {
        if (cycle_year == leap_years[i]) {
            return 1;
        }
    }
    return 0;
}

int islamic_days_in_month(int month, int year) {
    if (month < 1 || month > 12) {
        return -1;  // Error
    }
    
    // Odd months have 30 days, even months have 29 days
    if (month % 2 == 1) {
        return 30;
    } else if (month == 12 && islamic_is_leap_year(year)) {
        return 30;  // Last month has 30 days in leap year
    } else {
        return 29;
    }
}

CalendarResult islamic_validate_date(int day, int month, int year) {
    if (year < 1) {
        return CALENDAR_ERROR_INVALID_YEAR;
    }
    
    if (month < 1 || month > 12) {
        return CALENDAR_ERROR_INVALID_MONTH;
    }
    
    int max_days = islamic_days_in_month(month, year);
    if (day < 1 || day > max_days) {
        return CALENDAR_ERROR_INVALID_DATE;
    }
    
    return CALENDAR_SUCCESS;
}

IslamicDate* islamic_create_date(int day, int month, int year) {
    if (islamic_validate_date(day, month, year) != CALENDAR_SUCCESS) {
        return NULL;
    }
    
    IslamicDate* date = malloc(sizeof(IslamicDate));
    if (!date) {
        return NULL;
    }
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    date->is_leap_year = islamic_is_leap_year(year);
    strcpy(date->month_name, islamic_months[month - 1]);
    
    return date;
}

void islamic_destroy_date(IslamicDate* date) {
    if (date) {
        free(date);
    }
}

void islamic_print_month(int month, int year) {
    if (month < 1 || month > 12) {
        printf("Invalid month: %d\n", month);
        return;
    }
    
    printf("\n%s %d AH\n", islamic_months[month - 1], year);
    printf("Su Mo Tu We Th Fr Sa\n");
    printf("-------------------\n");
    
    // Convert 1st day of Islamic month to Gregorian to find day of week
    IslamicDate* first_day = islamic_create_date(1, month, year);
    if (!first_day) return;
    
    GregorianDate greg_date;
    if (islamic_to_gregorian(first_day, &greg_date) != CALENDAR_SUCCESS) {
        islamic_destroy_date(first_day);
        return;
    }
    
    int first_weekday = greg_date.day_of_week;
    int days_count = islamic_days_in_month(month, year);
    
    // Print leading spaces
    for (int i = 0; i < first_weekday; i++) {
        printf("   ");
    }
    
    // Print days
    for (int day = 1; day <= days_count; day++) {
        printf("%2d ", day);
        if ((day + first_weekday) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    islamic_destroy_date(first_day);
}

void islamic_print_date(const IslamicDate* date) {
    if (!date) {
        printf("NULL date\n");
        return;
    }
    
    printf("%d %s %d AH\n",
           date->base.day,
           date->month_name,
           date->base.year);
}

CalendarResult islamic_from_gregorian(const GregorianDate* greg_date, IslamicDate* islamic_date) {
    if (!greg_date || !islamic_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Calculate days since Islamic epoch
    double days_since_epoch = greg_date->julian_day - ISLAMIC_EPOCH_JD;
    
    if (days_since_epoch < 0) {
        return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    // Approximate Islamic year
    int year = (int)(days_since_epoch / 354.367) + 1;
    
    // Calculate start of Islamic year
    double year_start = (year - 1) * 354.367;
    double days_in_year = days_since_epoch - year_start;
    
    // Find month
    int month = 1;
    while (days_in_year >= islamic_days_in_month(month, year)) {
        days_in_year -= islamic_days_in_month(month, year);
        month++;
        if (month > 12) {
            month = 1;
            year++;
            year_start = (year - 1) * 354.367;
            days_in_year = days_since_epoch - year_start;
        }
    }
    
    int day = (int)days_in_year + 1;
    
    islamic_date->base.day = day;
    islamic_date->base.month = month;
    islamic_date->base.year = year;
    islamic_date->is_leap_year = islamic_is_leap_year(year);
    strcpy(islamic_date->month_name, islamic_months[month - 1]);
    
    return CALENDAR_SUCCESS;
}

CalendarResult islamic_to_gregorian(const IslamicDate* islamic_date, GregorianDate* greg_date) {
    if (!islamic_date || !greg_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Calculate total days since Islamic epoch
    double total_days = (islamic_date->base.year - 1) * 354.367;
    
    // Add days for completed months in current year
    for (int month = 1; month < islamic_date->base.month; month++) {
        total_days += islamic_days_in_month(month, islamic_date->base.year);
    }
    
    // Add days in current month
    total_days += islamic_date->base.day - 1;
    
    // Convert to Julian Day Number
    long jdn = (long)(ISLAMIC_EPOCH_JD + total_days);
    
    // Convert to Gregorian
    return gregorian_from_julian_day(jdn, greg_date);
}