// src/timezone/timezone.h
#ifndef TIMEZONE_H
#define TIMEZONE_H

#include "../../include/calendar_types.h"
#include <time.h>

// Time zone structure
typedef struct {
    char id[50];            // e.g., "America/New_York"
    char name[100];         // e.g., "Eastern Standard Time"
    char abbreviation[10];  // e.g., "EST"
    int utc_offset_minutes; // Offset from UTC in minutes
    int dst_offset_minutes; // DST offset in minutes
    int observes_dst;       // 1 if observes daylight saving time
} TimeZone;

// Enhanced date-time structure
typedef struct {
    Date date;
    int hour;
    int minute;
    int second;
    TimeZone timezone;
    long julian_day_number;
    double julian_day_fraction;
} DateTime;

// Time zone functions
TimeZone* timezone_create(const char* id, const char* name, int utc_offset_minutes);
void timezone_destroy(TimeZone* tz);
TimeZone* timezone_get_by_id(const char* id);
TimeZone* timezone_get_local(void);

// Date-time functions
DateTime* datetime_create(int year, int month, int day, int hour, int minute, int second, const TimeZone* tz);
void datetime_destroy(DateTime* dt);
DateTime* datetime_convert_timezone(const DateTime* dt, const TimeZone* target_tz);
DateTime* datetime_now_utc(void);
DateTime* datetime_now_local(void);

// DST calculations
int timezone_is_dst_active(const TimeZone* tz, const Date* date);
DateTime* timezone_get_dst_start(const TimeZone* tz, int year);
DateTime* timezone_get_dst_end(const TimeZone* tz, int year);

#endif // TIMEZONE_H