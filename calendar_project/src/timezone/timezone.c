// src/timezone/timezone.c (Fixed version)
#include "timezone.h"
#include "../utils/date_utils.h"    // Add this include
#include "../utils/string_utils.h"
#include "../calendars/gregorian.h" // Add this include

TimeZone* timezone_create(const char* id, const char* name, int utc_offset_minutes) {
    TimeZone* tz = malloc(sizeof(TimeZone));
    if (!tz) return NULL;
    
    string_safe_copy(tz->id, id, sizeof(tz->id));
    string_safe_copy(tz->name, name, sizeof(tz->name));
    tz->utc_offset_minutes = utc_offset_minutes;
    tz->dst_offset_minutes = 60; // Default 1 hour DST
    tz->observes_dst = 1; // Default to observing DST
    
    return tz;
}

void timezone_destroy(TimeZone* tz) {
    if (tz) free(tz);
}

TimeZone* timezone_get_by_id(const char* id) {
    (void)id; // Suppress unused parameter warning
    
    // Stub implementation - return UTC
    return timezone_create("UTC", "Coordinated Universal Time", 0);
}

TimeZone* timezone_get_local(void) {
    // Stub implementation - return UTC
    return timezone_create("UTC", "Coordinated Universal Time", 0);
}

DateTime* datetime_create(int year, int month, int day, int hour, int minute, int second, const TimeZone* tz) {
    DateTime* dt = malloc(sizeof(DateTime));
    if (!dt) return NULL;
    
    dt->date.year = year;
    dt->date.month = month;
    dt->date.day = day;
    dt->hour = hour;
    dt->minute = minute;
    dt->second = second;
    
    if (tz) {
        dt->timezone = *tz;
    } else {
        // Use UTC as default
        strcpy(dt->timezone.id, "UTC");
        strcpy(dt->timezone.name, "Coordinated Universal Time");
        dt->timezone.utc_offset_minutes = 0;
        dt->timezone.dst_offset_minutes = 0;
        dt->timezone.observes_dst = 0;
    }
    
    // Calculate Julian day
    dt->julian_day_number = julian_day_from_date(day, month, year);
    dt->julian_day_fraction = (hour + minute/60.0 + second/3600.0) / 24.0;
    
    return dt;
}

void datetime_destroy(DateTime* dt) {
    if (dt) free(dt);
}

DateTime* datetime_convert_timezone(const DateTime* dt, const TimeZone* target_tz) {
    (void)dt;        // Suppress unused parameter warning
    (void)target_tz; // Suppress unused parameter warning
    
    // Stub implementation
    return NULL;
}

DateTime* datetime_now_utc(void) {
    time_t now = time(NULL);
    struct tm *utc_tm = gmtime(&now);
    
    return datetime_create(
        utc_tm->tm_year + 1900,
        utc_tm->tm_mon + 1,
        utc_tm->tm_mday,
        utc_tm->tm_hour,
        utc_tm->tm_min,
        utc_tm->tm_sec,
        NULL  // UTC
    );
}

DateTime* datetime_now_local(void) {
    time_t now = time(NULL);
    struct tm *local_tm = localtime(&now);
    
    TimeZone* local_tz = timezone_get_local();
    DateTime* dt = datetime_create(
        local_tm->tm_year + 1900,
        local_tm->tm_mon + 1,
        local_tm->tm_mday,
        local_tm->tm_hour,
        local_tm->tm_min,
        local_tm->tm_sec,
        local_tz
    );
    
    timezone_destroy(local_tz);
    return dt;
}

int timezone_is_dst_active(const TimeZone* tz, const Date* date) {
    (void)tz;   // Suppress unused parameter warning
    (void)date; // Suppress unused parameter warning
    
    // Stub implementation
    return 0;
}

DateTime* timezone_get_dst_start(const TimeZone* tz, int year) {
    (void)tz;   // Suppress unused parameter warning
    (void)year; // Suppress unused parameter warning
    
    // Stub implementation
    return NULL;
}

DateTime* timezone_get_dst_end(const TimeZone* tz, int year) {
    (void)tz;   // Suppress unused parameter warning
    (void)year; // Suppress unused parameter warning
    
    // Stub implementation
    return NULL;
}