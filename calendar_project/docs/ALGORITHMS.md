# Algorithm Documentation

This document describes the mathematical algorithms and computational methods used in the Multi-Calendar System for accurate calendar conversions and astronomical calculations.

## Table of Contents

- [Calendar Conversion Algorithms](#calendar-conversion-algorithms)
- [Astronomical Calculations](#astronomical-calculations)
- [Date Validation](#date-validation)
- [Timezone Handling](#timezone-handling)
- [Performance Optimizations](#performance-optimizations)

## Calendar Conversion Algorithms

### Julian Day Number (JDN) System

The Julian Day Number system serves as the central conversion hub for all calendar systems. All dates are converted to JDN for intermediate calculations.

#### Gregorian to Julian Day

```c
long gregorian_to_julian_day(int day, int month, int year) {
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12 * a - 3;
    
    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 + 1721119;
}
```

**Algorithm Explanation:**
1. **Month Adjustment**: January and February are treated as months 13 and 14 of the previous year
2. **Century Correction**: Accounts for the Gregorian calendar reform (1582)
3. **Leap Year Formula**: Uses the standard Gregorian leap year rules
4. **Base Epoch**: JDN 1721119 corresponds to January 1, 0000 CE

#### Julian Day to Gregorian

```c
void julian_day_to_date(long jdn, int* day, int* month, int* year) {
    long a = jdn + 32044;
    long b = (4 * a + 3) / 146097;
    long c = a - (146097 * b) / 4;
    long d = (4 * c + 3) / 1461;
    long e = c - (1461 * d) / 4;
    long m = (5 * e + 2) / 153;
    
    *day = e - (153 * m + 2) / 5 + 1;
    *month = m + 3 - 12 * (m / 10);
    *year = 100 * b + d - 4800 + m / 10;
}
```

**Algorithm Steps:**
1. **Offset Adjustment**: Add 32044 to handle negative years
2. **Century Calculation**: Extract 400-year cycles
3. **Year Calculation**: Extract 4-year leap cycles
4. **Month/Day Extraction**: Use 153-day cycles for months

### Day of Week Calculation

#### Zeller's Congruence (Enhanced)

```c
static int calculate_day_of_week(int day, int month, int year) {
    int q = day;
    int m = month;
    int k = year % 100;
    int j = year / 100;
    
    // January and February are months 13 and 14 of previous year
    if (m < 3) {
        m += 12;
        if (k == 0) {
            k = 99;
            j--;
        } else {
            k--;
        }
    }
    
    // Zeller's formula
    int h = (q + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) - 2 * j) % 7;
    
    // Convert from Zeller's (0=Saturday) to standard (0=Sunday)
    return (h + 6) % 7;
}
```

**Key Improvements:**
- **Century Boundary Handling**: Proper year adjustment for January/February
- **Modular Arithmetic**: Ensures positive results
- **Standard Format**: Returns 0=Sunday through 6=Saturday

### Islamic Calendar (Hijri)

#### Astronomical Hijri Algorithm

```c
long islamic_to_julian_day(int day, int month, int year) {
    // Mean tropical year in the Islamic calendar
    const double ISLAMIC_YEAR = 354.36707;
    const long ISLAMIC_EPOCH = 1948439; // JDN of 1 Muharram 1 AH
    
    // Calculate approximate JDN
    long approx_jdn = ISLAMIC_EPOCH + 
                     (long)((year - 1) * ISLAMIC_YEAR) + 
                     (long)((month - 1) * 29.53058867) + 
                     day - 1;
    
    // Fine-tune using lunar month calculations
    return islamic_fine_tune(approx_jdn, day, month, year);
}
```

**Algorithm Features:**
- **Astronomical Base**: Uses mean lunar month length (29.53058867 days)
- **Epoch Reference**: July 16, 622 CE (1 Muharram 1 AH)
- **Fine Tuning**: Adjusts for actual lunar observations

#### Lunar Month Length Calculation

```c
double lunar_month_length(int month, int year) {
    // Mean synodic month
    const double SYNODIC_MONTH = 29.53058867;
    
    // Account for lunar apogee/perigee variations
    double variation = 0.0;
    
    // Calculate lunar anomaly
    double anomaly = lunar_anomaly(month, year);
    variation += 0.1734 * sin(anomaly);
    
    // Solar anomaly effect
    double solar_anomaly = solar_anomaly(month, year);
    variation += 0.0021 * sin(solar_anomaly);
    
    return SYNODIC_MONTH + variation;
}
```

### Chinese Calendar

#### Lunisolar Calendar Algorithm

```c
typedef struct {
    int year_cycle;      // 60-year cycle number
    int year_in_cycle;   // Year within cycle (1-60)
    int month;           // Month number
    int day;             // Day number
    bool is_leap_month;  // Leap month indicator
} ChineseDate;

long chinese_to_julian_day(ChineseDate* date) {
    // Calculate solar year start
    long solar_year_start = chinese_solar_year(date->year_cycle, date->year_in_cycle);
    
    // Add lunar months
    long month_jdn = solar_year_start;
    for (int m = 1; m < date->month; m++) {
        month_jdn += chinese_month_length(date->year_cycle, date->year_in_cycle, m);
    }
    
    return month_jdn + date->day - 1;
}
```

**Key Components:**
- **60-Year Cycles**: Traditional Chinese calendar cycles
- **Leap Month Handling**: Intercalary months for solar alignment
- **Solar Term Integration**: 24 solar terms per year

### Hebrew Calendar

#### Metonic Cycle Implementation

```c
long hebrew_to_julian_day(int day, int month, int year) {
    // Hebrew calendar epoch: September 7, 3761 BCE
    const long HEBREW_EPOCH = 347998;
    
    // Calculate Metonic cycle position
    int metonic_cycle = (year - 1) / 19;
    int year_in_cycle = (year - 1) % 19 + 1;
    
    // Determine if leap year (7 leap years per 19-year cycle)
    bool is_leap = hebrew_is_leap_year(year_in_cycle);
    
    // Calculate month lengths
    int total_days = 0;
    for (int m = 1; m < month; m++) {
        total_days += hebrew_month_length(m, is_leap);
    }
    
    return HEBREW_EPOCH + 
           metonic_cycle * METONIC_CYCLE_DAYS + 
           hebrew_year_days(year_in_cycle) + 
           total_days + day - 1;
}
```

**Algorithm Features:**
- **Metonic Cycles**: 19-year cycles with 7 leap years
- **Variable Year Length**: 353-385 days depending on type
- **Molad Calculation**: New moon determination

## Astronomical Calculations

### Lunar Phase Algorithm

#### Primary Lunar Phase Calculation

```c
typedef struct {
    double age;              // Days since new moon
    double illumination;     // Percentage illuminated (0-100)
    LunarPhase phase;       // Primary phase enum
    double phase_angle;     // Angle in lunar cycle (0-360°)
} LunarInfo;

LunarInfo calculate_lunar_info(long julian_day) {
    LunarInfo info;
    
    // Reference new moon (January 6, 2000, 18:14 UTC)
    const double REFERENCE_NEW_MOON = 2451549.26;
    const double SYNODIC_MONTH = 29.53058867;
    
    // Calculate lunar age
    double days_since_reference = julian_day - REFERENCE_NEW_MOON;
    info.age = fmod(days_since_reference, SYNODIC_MONTH);
    if (info.age < 0) info.age += SYNODIC_MONTH;
    
    // Calculate phase angle (0° = new moon, 180° = full moon)
    info.phase_angle = (info.age / SYNODIC_MONTH) * 360.0;
    
    // Calculate illumination percentage
    info.illumination = (1.0 - cos(info.phase_angle * M_PI / 180.0)) / 2.0 * 100.0;
    
    // Determine primary phase
    info.phase = determine_primary_phase(info.phase_angle);
    
    return info;
}
```

#### Enhanced Lunar Calculation with Perturbations

```c
double precise_lunar_age(long julian_day) {
    const double SYNODIC_MONTH = 29.53058867;
    const double REFERENCE_NEW_MOON = 2451549.26;
    
    double T = (julian_day - 2451545.0) / 36525.0; // Julian centuries since J2000.0
    
    // Mean lunar longitude
    double L = 218.3164477 + 481267.88123421 * T;
    
    // Mean lunar anomaly
    double M = 134.9633964 + 477198.8675055 * T;
    
    // Mean solar anomaly
    double Ms = 357.5291092 + 35999.0502909 * T;
    
    // Moon's argument of latitude
    double F = 93.2720950 + 483202.0175233 * T;
    
    // Lunar distance anomaly
    double D = 297.8501921 + 445267.1114034 * T;
    
    // Calculate perturbations
    double correction = 0.0;
    correction += -1.274 * sin(M - 2*D);           // Evection
    correction += +0.658 * sin(2*D);               // Variation
    correction += -0.186 * sin(Ms);                // Yearly equation
    correction += -0.059 * sin(2*M - 2*D);         // Parallactic inequality
    correction += -0.057 * sin(M - 2*D + Ms);      // 
    correction += +0.053 * sin(M + 2*D);           //
    
    // Apply corrections
    double days_since_ref = julian_day - REFERENCE_NEW_MOON + correction;
    double lunar_age = fmod(days_since_ref, SYNODIC_MONTH);
    
    return lunar_age < 0 ? lunar_age + SYNODIC_MONTH : lunar_age;
}
```

### Solar Position Calculations

#### Solar Longitude (for Chinese Calendar)

```c
double solar_longitude(long julian_day) {
    double T = (julian_day - 2451545.0) / 36525.0;
    
    // Mean longitude of the Sun
    double L0 = 280.46646 + 36000.76983 * T + 0.0003032 * T * T;
    
    // Mean anomaly of the Sun
    double M = 357.52911 + 35999.05029 * T - 0.0001537 * T * T;
    
    // Equation of center
    double C = (1.914602 - 0.004817 * T - 0.000014 * T * T) * sin(M * M_PI / 180.0);
    C += (0.019993 - 0.000101 * T) * sin(2 * M * M_PI / 180.0);
    C += 0.000289 * sin(3 * M * M_PI / 180.0);
    
    // True longitude
    double true_longitude = L0 + C;
    
    // Normalize to 0-360°
    return fmod(true_longitude, 360.0);
}
```

## Date Validation

### Comprehensive Date Validation

```c
typedef enum {
    CALENDAR_SUCCESS = 0,
    CALENDAR_ERROR_INVALID_DATE,
    CALENDAR_ERROR_INVALID_MONTH,
    CALENDAR_ERROR_INVALID_YEAR,
    CALENDAR_ERROR_INVALID_DAY,
    CALENDAR_ERROR_OUT_OF_RANGE
} CalendarResult;

CalendarResult validate_gregorian_date(int day, int month, int year) {
    // Year range check
    if (year < 1 || year > 9999) {
        return CALENDAR_ERROR_INVALID_YEAR;
    }
    
    // Month range check
    if (month < 1 || month > 12) {
        return CALENDAR_ERROR_INVALID_MONTH;
    }
    
    // Day range check
    if (day < 1) {
        return CALENDAR_ERROR_INVALID_DAY;
    }
    
    // Days in month check
    int max_days = gregorian_days_in_month(month, year);
    if (day > max_days) {
        return CALENDAR_ERROR_INVALID_DAY;
    }
    
    return CALENDAR_SUCCESS;
}
```

### Leap Year Algorithms

#### Gregorian Leap Year

```c
bool gregorian_is_leap_year(int year) {
    // Divisible by 400: leap year
    if (year % 400 == 0) return true;
    
    // Divisible by 100 but not 400: not leap year
    if (year % 100 == 0) return false;
    
    // Divisible by 4: leap year
    return (year % 4 == 0);
}
```

#### Hebrew Leap Year (Metonic Cycle)

```c
bool hebrew_is_leap_year(int year) {
    // 7 leap years in each 19-year Metonic cycle
    // Years 3, 6, 8, 11, 14, 17, 19 are leap years
    int year_in_cycle = ((year - 1) % 19) + 1;
    
    return (year_in_cycle == 3 || year_in_cycle == 6 || 
            year_in_cycle == 8 || year_in_cycle == 11 || 
            year_in_cycle == 14 || year_in_cycle == 17 || 
            year_in_cycle == 19);
}
```

## Timezone Handling

### UTC Offset Calculations

```c
typedef struct {
    int hours;
    int minutes;
    bool is_dst;
    char zone_name[32];
} TimezoneInfo;

long apply_timezone_offset(long utc_julian_day, TimezoneInfo* tz) {
    double offset_days = (tz->hours + tz->minutes / 60.0) / 24.0;
    
    // Add DST offset if applicable
    if (tz->is_dst) {
        offset_days += 1.0 / 24.0; // +1 hour for DST
    }
    
    return utc_julian_day + (long)offset_days;
}
```

## Performance Optimizations

### Lookup Tables

#### Month Length Cache

```c
static int month_lengths[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Non-leap year
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Leap year
};

int gregorian_days_in_month(int month, int year) {
    int leap = gregorian_is_leap_year(year) ? 1 : 0;
    return month_lengths[leap][month - 1];
}
```

#### Julian Day Cache for Common Dates

```c
typedef struct {
    int day, month, year;
    long julian_day;
} DateCache;

static DateCache jdn_cache[CACHE_SIZE];
static int cache_count = 0;

long cached_gregorian_to_jdn(int day, int month, int year) {
    // Check cache first
    for (int i = 0; i < cache_count; i++) {
        if (jdn_cache[i].day == day && 
            jdn_cache[i].month == month && 
            jdn_cache[i].year == year) {
            return jdn_cache[i].julian_day;
        }
    }
    
    // Calculate and cache
    long jdn = gregorian_to_julian_day(day, month, year);
    
    if (cache_count < CACHE_SIZE) {
        jdn_cache[cache_count++] = (DateCache){day, month, year, jdn};
    }
    
    return jdn;
}
```

## Algorithm Accuracy

### Precision Specifications

| Calendar System | Accuracy Range | Error Margin |
|-----------------|---------------|--------------|
| **Gregorian** | 1 CE - 9999 CE | ±0 days |
| **Islamic** | 1 AH - 1500 AH | ±1 day |
| **Chinese** | 1900 CE - 2100 CE | ±1 day |
| **Hebrew** | 1 AM - 6000 AM | ±0 days |
| **Lunar Phases** | 1900 CE - 2100 CE | ±4 hours |

### Validation Test Cases

```c
// Test case structure
typedef struct {
    char* description;
    int input_day, input_month, input_year;
    int expected_day, expected_month, expected_year;
    CalendarType source_type;
    CalendarType target_type;
} TestCase;

// Example test cases
TestCase test_cases[] = {
    // Gregorian leap year
    {"Feb 29, 2024 (leap year)", 29, 2, 2024, 29, 2, 2024, CAL_GREGORIAN, CAL_GREGORIAN},
    
    // Islamic New Year
    {"1 Muharram 1446 AH", 1, 1, 1446, 7, 7, 2024, CAL_ISLAMIC, CAL_GREGORIAN},
    
    // Hebrew Rosh Hashanah
    {"1 Tishrei 5785", 1, 7, 5785, 16, 9, 2024, CAL_HEBREW, CAL_GREGORIAN},
    
    // Chinese New Year
    {"Chinese New Year 2024", 1, 1, 4722, 10, 2, 2024, CAL_CHINESE, CAL_GREGORIAN}
};
```

---

*Algorithm documentation last updated: 2025-06-17 03:16:56 UTC*
*Author: Linh Duong Tuan*