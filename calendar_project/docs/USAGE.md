# Usage Guide

This comprehensive guide covers all aspects of using the Multi-Calendar System, from basic operations to advanced features.

## Table of Contents

- [Getting Started](#getting-started)
- [Command-Line Interface](#command-line-interface)
- [GUI Application](#gui-application)
- [Calendar Conversions](#calendar-conversions)
- [Astronomical Features](#astronomical-features)
- [Holiday Database](#holiday-database)
- [Export Functionality](#export-functionality)
- [Advanced Usage](#advanced-usage)
- [Troubleshooting](#troubleshooting)

## Getting Started

### First Run

After installation, the Multi-Calendar System can be accessed in two ways:

#### Command-Line Interface
```bash
# Launch interactive menu
calendar

# Direct commands
calendar --today
calendar --help
calendar --version
```

#### GUI Application (macOS)
```bash
# Launch from Applications folder
open /Applications/calendar_gui.app

# Or from command line
open -a "Multi-Calendar System"
```

### Basic Concepts

The system uses **Julian Day Numbers (JDN)** as the central conversion mechanism between calendar systems. All dates are internally converted to JDN for accurate calculations.

**Supported Calendars:**
- 📅 **Gregorian** - Standard Western calendar
- 🌙 **Islamic (Hijri)** - Lunar calendar used in Islamic countries
- 🐉 **Chinese** - Traditional lunisolar calendar
- ✡️ **Hebrew** - Jewish calendar system

## Command-Line Interface

### Interactive Menu

Launch the interactive menu system:

```bash
calendar
```

**Main Menu Options:**
```
=== MULTI-CALENDAR SYSTEM v2.0.0 ===

1. 📅 View Today's Date in All Calendars
2. 🔄 Convert Between Calendar Systems
3. 🌙 Lunar Phase Information
4. 🎉 Holiday and Cultural Events
5. 🌍 Timezone Conversions
6. 📁 Export Calendar Data
7. ❓ Help and Documentation
8. 🚪 Exit

Enter your choice [1-8]:
```

### Direct Commands

#### Show Today's Date

```bash
calendar --today
```

**Example Output:**
```
=== TODAY'S DATE: 2025-06-17 ===

📅 GREGORIAN: Tuesday, June 17, 2025
🌙 ISLAMIC:   20 Dhu al-Hijjah 1446 AH
🐉 CHINESE:   Year of Snake (Yi-Si), Month 5, Day 23
✡️ HEBREW:    22 Sivan 5785

🌙 LUNAR INFO:
Phase: Waning Gibbous
Age: 18.7 days
Illumination: 67.3%

🎉 HOLIDAYS TODAY:
- No major holidays

📍 TIMEZONE: UTC+0 (System Default)
```

#### Calendar Conversion

```bash
calendar --convert
```

**Interactive Conversion Process:**
```
=== CALENDAR CONVERSION ===

Select source calendar:
1. Gregorian (CE/BCE)
2. Islamic (AH)  
3. Chinese
4. Hebrew (AM)

Enter choice [1-4]: 1

Enter Gregorian date (DD MM YYYY): 17 06 2025

=== CONVERSION RESULTS ===

📅 SOURCE: Tuesday, June 17, 2025 (Gregorian)

🔄 CONVERSIONS:
🌙 Islamic:  20 Dhu al-Hijjah 1446 AH
🐉 Chinese:  Year of Snake, Month 5, Day 23
✡️ Hebrew:   22 Sivan 5785
📊 Julian:   JDN 2465952

🌙 ASTRONOMICAL DATA:
Phase: Waning Gibbous (67.3% illuminated)
Age: 18.7 days since new moon
Next New Moon: June 25, 2025
Next Full Moon: July 13, 2025
```

#### Lunar Information

```bash
calendar --moon
```

**Example Output:**
```
=== LUNAR PHASE INFORMATION ===

📅 Date: Tuesday, June 17, 2025
🌙 Current Phase: Waning Gibbous

📊 DETAILED INFORMATION:
Age: 18.7 days since new moon
Illumination: 67.3%
Phase Angle: 201.4°

🗓️ LUNAR CALENDAR:
Previous New Moon: May 30, 2025
Current Full Moon: June 14, 2025  
Next New Moon: June 25, 2025
Next Full Moon: July 13, 2025

🌙 UPCOMING PHASES:
June 22: Last Quarter
June 25: New Moon
July 02: First Quarter
July 13: Full Moon
```

#### Holiday Information

```bash
calendar --holidays
```

**Example Output:**
```
=== CULTURAL HOLIDAYS & EVENTS ===

📅 UPCOMING HOLIDAYS (Next 30 days):

🌍 INTERNATIONAL:
June 21: Summer Solstice (Astronomical)
June 23: International Olympic Day

🇺🇸 UNITED STATES:
July 4: Independence Day

🕌 ISLAMIC CALENDAR:
July 17: Muharram 1447 AH (Islamic New Year)
July 26: Day of Ashura

✡️ HEBREW CALENDAR:
July 13: 17 Tammuz (Fast Day)
August 3: Tu B'Av (Festival of Love)

🐉 CHINESE CALENDAR:
July 20: Ghost Festival (Zhongyuan)

📊 Total holidays found: 8
```

### Command-Line Options

#### Display Options

```bash
# Show version information
calendar --version

# Display help
calendar --help

# Verbose output
calendar --verbose --today

# Quiet mode (minimal output)
calendar --quiet --convert
```

#### Format Options

```bash
# JSON output format
calendar --format json --today

# CSV format
calendar --format csv --holidays

# XML format
calendar --format xml --moon
```

#### Date Range Operations

```bash
# Show holidays in date range
calendar --holidays --from 2025-06-01 --to 2025-12-31

# Convert date range
calendar --convert --range 2025-01-01:2025-12-31 --target islamic

# Export date range
calendar --export csv --range 2025-06-01:2025-06-30 --file june_2025.csv
```

## GUI Application

### Overview

The GUI application provides a beautiful, native macOS interface with three main tabs:

1. **📅 Calendar Tab** - Live date conversions and calculations
2. **🎉 Holidays Tab** - Cultural celebrations and events browser
3. **📁 Export Tab** - Data export with various format options

### Calendar Tab

The main calendar interface provides:

#### Date Input Section
- **Date Picker**: Select any date from 1 CE to 9999 CE
- **Today Button**: Jump to current date
- **Calendar Grid**: Visual date selection

#### Conversion Display
- **Real-time Updates**: Automatic conversion as you change dates
- **Multiple Formats**: All four calendar systems displayed simultaneously
- **Day of Week**: Accurate day-of-week calculation
- **Julian Day**: JDN display for reference

#### Astronomical Information
- **Current Moon Phase**: Visual moon phase indicator
- **Lunar Age**: Days since new moon
- **Illumination**: Percentage of moon visible
- **Phase Transitions**: Next phase dates

**Calendar Tab Layout:**
```
┌─────────────────────────────────────────────────────┐
│ 📅 Multi-Calendar System                            │
├─────────────────────────────────────────────────────┤
│ Date: [  17  ] [  June ▼  ] [  2025  ]    [Today]  │
├─────────────────────────────────────────────────────┤
│ 📅 Gregorian: Tuesday, June 17, 2025               │
│ 🌙 Islamic:   20 Dhu al-Hijjah 1446 AH            │
│ 🐉 Chinese:   Year of Snake, Month 5, Day 23      │
│ ✡️ Hebrew:    22 Sivan 5785                        │
│ 📊 Julian:    JDN 2465952                          │
├─────────────────────────────────────────────────────┤
│ 🌙 Moon: Waning Gibbous (67.3%) - Age 18.7 days   │
│ Next: New Moon on June 25, 2025                    │
└─────────────────────────────────────────────────────┘
```

### Holidays Tab

Browse and search cultural holidays and events:

#### Search and Filter
- **Date Range**: Select start and end dates
- **Culture Filter**: Filter by culture (Islamic, Hebrew, Chinese, etc.)
- **Country Filter**: Filter by country or region
- **Search Text**: Search holiday names and descriptions

#### Holiday Display
- **List View**: Scrollable list of holidays
- **Detail View**: Detailed information for selected holiday
- **Calendar Integration**: Add holidays to system calendar

**Holidays Tab Features:**
```
┌─────────────────────────────────────────────────────┐
│ 🎉 Cultural Holidays & Events                       │
├─────────────────────────────────────────────────────┤
│ Search: [____________] 🔍                           │
│ Filter: [All Cultures ▼] [All Countries ▼]         │
│ Range: [2025-06-01] to [2025-12-31]                │
├─────────────────────────────────────────────────────┤
│ ☰ Holiday List                 │ 📋 Details         │
│ ┌─────────────────────────────┐ │ ┌─────────────────┐ │
│ │ 📅 June 21, 2025           │ │ │ Summer Solstice │ │
│ │ 🌞 Summer Solstice          │ │ │                 │ │
│ │ ─────────────────────────── │ │ │ Date: June 21   │ │
│ │ 📅 July 4, 2025            │ │ │ Type: Astronom. │ │
│ │ 🇺🇸 Independence Day        │ │ │ Global event    │ │
│ │ ─────────────────────────── │ │ │                 │ │
│ │ 📅 July 17, 2025           │ │ │ The summer      │ │
│ │ 🕌 Islamic New Year         │ │ │ solstice marks  │ │
│ └─────────────────────────────┘ │ │ the longest...  │ │
│                                 │ └─────────────────┘ │
└─────────────────────────────────────────────────────┘
```

### Export Tab

Export calendar data in multiple formats:

#### Export Options
- **Date Range**: Select range to export
- **Calendars**: Choose which calendar systems to include
- **Format**: CSV, iCal, JSON, or XML
- **Options**: Include holidays, lunar info, etc.

#### Export Formats

**CSV Export Options:**
- Date columns for each calendar system
- Julian Day Numbers
- Day of week information
- Lunar phase data
- Holiday information

**iCal Export Options:**
- Standard calendar format
- Compatible with Apple Calendar, Google Calendar
- Holiday events included
- Lunar phase events

### GUI Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `⌘+T` | Go to today |
| `⌘+E` | Export current view |
| `⌘+F` | Search holidays |
| `⌘+R` | Refresh data |
| `⌘+,` | Preferences |
| `⌘+?` | Help |
| `⌘+Q` | Quit application |
| `←/→` | Navigate days |
| `↑/↓` | Navigate months |
| `⌘+←/→` | Navigate years |

## Calendar Conversions

### Gregorian Calendar

The Gregorian calendar is the most widely used calendar system worldwide.

#### Features
- **Leap Years**: Every 4 years, except century years not divisible by 400
- **Accuracy**: Proleptic Gregorian (extends backwards before 1582 CE)
- **Range**: 1 CE to 9999 CE

#### Usage Examples

```bash
# Interactive conversion
calendar --convert

# Select source: Gregorian
# Enter date: 29 02 2024
# Result: Valid leap year date

# Convert Christmas 2025
# Input: 25 12 2025
# Output: Islamic: 24 Jumada al-Thani 1447 AH
#         Chinese: Year of Snake, Month 11, Day 25
#         Hebrew: 4 Tevet 5786
```

### Islamic Calendar (Hijri)

The Islamic calendar is a purely lunar calendar used throughout the Islamic world.

#### Features
- **Lunar Months**: 29 or 30 days alternating
- **Year Length**: ~354 days (11 days shorter than solar year)
- **Epoch**: July 16, 622 CE (1 Muharram 1 AH)

#### Month Names
```
1. Muharram          7. Rajab
2. Safar             8. Sha'ban
3. Rabi' al-Awwal    9. Ramadan
4. Rabi' al-Thani   10. Shawwal
5. Jumada al-Awwal  11. Dhu al-Qi'dah
6. Jumada al-Thani  12. Dhu al-Hijjah
```

#### Usage Examples

```bash
# Convert Islamic New Year
# Input: 1 1 1447 (1 Muharram 1447 AH)
# Output: Gregorian: July 7, 2025

# Convert Ramadan start
# Input: 1 9 1446 (1 Ramadan 1446 AH)
# Output: Gregorian: March 1, 2025
```

### Chinese Calendar

The Chinese calendar is a lunisolar calendar with complex rules for leap months.

#### Features
- **Zodiac Animals**: 12-year cycle (Rat, Ox, Tiger, Rabbit, Dragon, Snake, Horse, Goat, Monkey, Rooster, Dog, Pig)
- **Elements**: 5 elements rotating every 2 years
- **Sexagenary Cycle**: 60-year cycle combining animals and elements

#### Zodiac Calculation

```bash
# Find zodiac for current year
calendar --chinese --zodiac 2025
# Output: Year of the Snake (Yi-Si)

# Get full cycle information
calendar --chinese --cycle 2025
# Output: Cycle 78, Year 42 of 60
#         Element: Wood
#         Animal: Snake
```

#### Usage Examples

```bash
# Convert Chinese New Year 2025
# Input: 1 1 4722 (Chinese calendar)
# Output: Gregorian: January 29, 2025
#         Zodiac: Year of the Snake begins

# Convert Mid-Autumn Festival
# Input: 15 8 4722 (15th day, 8th month)
# Output: Gregorian: September 6, 2025
```

### Hebrew Calendar

The Hebrew calendar is a lunisolar calendar used for Jewish religious observances.

#### Features
- **Metonic Cycle**: 19-year cycle with 7 leap years
- **Variable Year**: 353-385 days depending on type
- **Epoch**: September 7, 3761 BCE (1 Tishrei 1 AM)

#### Month Names
```
1. Tishrei    7. Nisan
2. Heshvan    8. Iyar
3. Kislev     9. Sivan
4. Tevet     10. Tammuz
5. Shevat    11. Av
6. Adar      12. Elul
             13. Adar II (leap years only)
```

#### Usage Examples

```bash
# Convert Rosh Hashanah 5786
# Input: 1 7 5786 (1 Tishrei 5786)
# Output: Gregorian: September 23, 2025

# Convert Passover 5785
# Input: 15 7 5785 (15 Nisan 5785)
# Output: Gregorian: April 13, 2025
```

## Astronomical Features

### Lunar Phase Tracking

#### Phase Categories

```
🌑 New Moon          (0% illuminated)
🌒 Waxing Crescent   (0-50% illuminated, growing)
🌓 First Quarter     (~50% illuminated)
🌔 Waxing Gibbous    (50-100% illuminated, growing)
🌕 Full Moon         (100% illuminated)
🌖 Waning Gibbous    (50-100% illuminated, shrinking)
🌗 Last Quarter      (~50% illuminated)
🌘 Waning Crescent   (0-50% illuminated, shrinking)
```

#### Lunar Calculations

```bash
# Get detailed lunar information
calendar --moon --detailed

# Output:
# 🌙 LUNAR PHASE REPORT
# Date: Tuesday, June 17, 2025
# Phase: Waning Gibbous
# Age: 18.7 days since new moon
# Illumination: 67.3%
# Phase Angle: 201.4°
# 
# 🗓️ PHASE TRANSITIONS:
# Previous New Moon: May 30, 2025 09:12 UTC
# Previous Full Moon: June 14, 2025 02:41 UTC
# Next Last Quarter: June 22, 2025 14:08 UTC
# Next New Moon: June 25, 2025 10:32 UTC
```

#### Lunar Calendar

```bash
# Show lunar calendar for a month
calendar --lunar-calendar 2025-06

# Output: Calendar grid showing phases for each day
```

### Solar Position

The system can calculate solar position for various calendar systems, particularly useful for the Chinese calendar's solar terms.

#### Solar Terms (Chinese Calendar)

```bash
# Get solar terms for a year
calendar --solar-terms 2025

# Output: 24 solar terms with dates
# 立春 (Lichun - Beginning of Spring): February 3, 2025
# 雨水 (Yushui - Rain Water): February 18, 2025
# ...
```

## Holiday Database

### Supported Holiday Categories

#### International Holidays
- New Year's Day
- International Women's Day
- Earth Day
- World Health Day
- Human Rights Day

#### Religious Observances

**Islamic Holidays:**
- Eid al-Fitr (End of Ramadan)
- Eid al-Adha (Festival of Sacrifice)
- Mawlid an-Nabi (Prophet's Birthday)
- Islamic New Year (1 Muharram)
- Day of Ashura (10 Muharram)

**Jewish Holidays:**
- Rosh Hashanah (New Year)
- Yom Kippur (Day of Atonement)
- Sukkot (Festival of Booths)
- Passover (Pesach)
- Shavuot (Festival of Weeks)

**Chinese Traditional:**
- Chinese New Year (Spring Festival)
- Qingming Festival (Tomb Sweeping)
- Dragon Boat Festival
- Mid-Autumn Festival
- Double Ninth Festival

#### National Holidays

**United States:**
- Independence Day (July 4)
- Thanksgiving Day
- Memorial Day
- Labor Day

**Other Countries:**
- Canada Day (July 1)
- Australia Day (January 26)
- Diwali (India)
- Guy Fawkes Night (UK)

### Holiday Search

#### Search by Date

```bash
# Find holidays on a specific date
calendar --holidays --date 2025-07-04

# Output:
# 🎆 July 4, 2025 (Friday)
# 🇺🇸 Independence Day (United States)
# Description: Celebrates the adoption of the Declaration of Independence in 1776
```

#### Search by Name

```bash
# Search for specific holiday
calendar --holidays --search "Christmas"

# Output:
# 🎄 December 25, 2025 (Thursday)
# 🇺🇸🇬🇧🇨🇦 Christmas Day
# Description: Christian holiday celebrating the birth of Jesus Christ
```

#### Search by Culture

```bash
# Find Islamic holidays
calendar --holidays --culture islamic --year 2025

# Output:
# 🕌 ISLAMIC HOLIDAYS 2025:
# March 30: Eid al-Fitr 1446 AH
# June 6: Eid al-Adha 1446 AH
# July 7: Islamic New Year 1447 AH
# September 15: Mawlid an-Nabi 1447 AH
```

## Export Functionality

### CSV Export

#### Basic CSV Export

```bash
# Export current month to CSV
calendar --export csv --month 2025-06 --file june_2025.csv
```

**CSV Format:**
```csv
Date,Gregorian,Islamic,Chinese,Hebrew,JulianDay,DayOfWeek,LunarPhase,Illumination,Holidays
2025-06-01,"June 1, 2025","5 Dhu al-Hijjah 1446","Month 4, Day 7, Year of Snake","17 Sivan 5785",2465937,Sunday,"Waning Crescent",15.2%,""
2025-06-17,"June 17, 2025","20 Dhu al-Hijjah 1446","Month 5, Day 23, Year of Snake","22 Sivan 5785",2465952,Tuesday,"Waning Gibbous",67.3%,""
```

#### Advanced CSV Options

```bash
# Export with all features
calendar --export csv \
         --range 2025-01-01:2025-12-31 \
         --include-lunar \
         --include-holidays \
         --include-zodiac \
         --file full_year_2025.csv
```

### iCal Export

#### Basic iCal Export

```bash
# Export holidays as calendar events
calendar --export ical --holidays --year 2025 --file holidays_2025.ics
```

**iCal Format:**
```ics
BEGIN:VCALENDAR
VERSION:2.0
PRODID:-//Multi-Calendar System//v2.0.0//EN
BEGIN:VEVENT
UID:20250704-independence-day@multi-calendar-system
DTSTART;VALUE=DATE:20250704
SUMMARY:Independence Day
DESCRIPTION:Celebrates the adoption of the Declaration of Independence in 1776
CATEGORIES:NATIONAL,UNITED_STATES
END:VEVENT
END:VCALENDAR
```

#### Lunar Phase iCal

```bash
# Export lunar phases as events
calendar --export ical --lunar-phases --year 2025 --file lunar_2025.ics
```

### JSON Export

#### Structured Data Export

```bash
# Export as JSON for developers
calendar --export json --range 2025-06-01:2025-06-30 --file june_2025.json
```

**JSON Format:**
```json
{
  "export_info": {
    "version": "2.0.0",
    "generated": "2025-06-17T03:16:56Z",
    "range": {
      "start": "2025-06-01",
      "end": "2025-06-30"
    }
  },
  "dates": [
    {
      "gregorian": {
        "date": "2025-06-17",
        "day_of_week": "Tuesday",
        "julian_day": 2465952
      },
      "islamic": {
        "date": "1446-12-20",
        "month_name": "Dhu al-Hijjah",
        "year": 1446
      },
            "chinese": {
        "year": 4722,
        "month": 5,
        "day": 23,
        "zodiac": "Snake",
        "element": "Wood",
        "stem_branch": "Yi-Si"
      },
      "hebrew": {
        "date": "5785-09-22",
        "month_name": "Sivan",
        "year": 5785,
        "is_leap_year": false
      },
      "lunar": {
        "phase": "Waning Gibbous",
        "age": 18.7,
        "illumination": 67.3,
        "next_new_moon": "2025-06-25"
      },
      "holidays": []
    }
  ]
}
```

### XML Export

#### Enterprise-Compatible Format

```bash
# Export as XML for enterprise systems
calendar --export xml --range 2025-06-01:2025-06-30 --file june_2025.xml
```

**XML Format:**
```xml
<?xml version="1.0" encoding="UTF-8"?>
<CalendarExport version="2.0.0" generated="2025-06-17T03:22:17Z">
  <ExportInfo>
    <DateRange start="2025-06-01" end="2025-06-30"/>
    <IncludedSystems>
      <System>Gregorian</System>
      <System>Islamic</System>
      <System>Chinese</System>
      <System>Hebrew</System>
    </IncludedSystems>
  </ExportInfo>
  <Dates>
    <Date>
      <Gregorian date="2025-06-17" dayOfWeek="Tuesday" julianDay="2465952"/>
      <Islamic date="1446-12-20" monthName="Dhu al-Hijjah" year="1446"/>
      <Chinese year="4722" month="5" day="23" zodiac="Snake" element="Wood"/>
      <Hebrew date="5785-09-22" monthName="Sivan" year="5785" isLeapYear="false"/>
      <Lunar phase="Waning Gibbous" age="18.7" illumination="67.3"/>
      <Holidays/>
    </Date>
  </Dates>
</CalendarExport>
```

## Advanced Usage

### Batch Operations

#### Converting Multiple Dates

```bash
# Create a file with dates to convert
echo "17 06 2025
25 12 2025
01 01 2026" > dates.txt

# Batch convert
calendar --batch-convert --input dates.txt --output conversions.csv --source gregorian --target all
```

#### Holiday Lookup for Date Range

```bash
# Find all holidays in a year
calendar --holidays --year 2025 --culture all --export csv --file holidays_2025.csv
```

### Custom Configuration

#### Configuration File

Create `~/.calendar-config` for custom settings:

```ini
[display]
default_format=long
show_julian_day=true
show_lunar_phase=true
date_separator=/

[calendars]
default_calendar=gregorian
enable_islamic=true
enable_chinese=true
enable_hebrew=true

[lunar]
precision_hours=4
show_illumination=true
show_age=true

[holidays]
include_cultural=true
include_religious=true
include_national=true
default_country=US

[export]
default_format=csv
include_headers=true
date_format=iso
```

#### Environment Variables

```bash
# Set default timezone
export CALENDAR_TIMEZONE="America/New_York"

# Set default output format
export CALENDAR_FORMAT="json"

# Enable verbose logging
export CALENDAR_DEBUG=1

# Set custom data directory
export CALENDAR_DATA_DIR="/usr/local/share/calendar"
```

### Programming Interface

#### Using as a Library

```c
#include "calendar.h"

int main() {
    // Initialize the calendar system
    calendar_init();
    
    // Create a Gregorian date
    GregorianDate* date = gregorian_create_date(17, 6, 2025);
    if (!date) {
        fprintf(stderr, "Failed to create date\n");
        return 1;
    }
    
    // Convert to Islamic calendar
    IslamicDate* islamic_date;
    if (gregorian_to_islamic(date, &islamic_date) == CALENDAR_SUCCESS) {
        printf("Islamic date: %d %s %d AH\n",
               islamic_date->base.day,
               islamic_month_names[islamic_date->base.month - 1],
               islamic_date->base.year);
    }
    
    // Get lunar information
    LunarInfo lunar = calculate_lunar_info(date->julian_day);
    printf("Moon phase: %s (%.1f%% illuminated)\n",
           lunar_phase_names[lunar.phase],
           lunar.illumination);
    
    // Clean up
    gregorian_destroy_date(date);
    islamic_destroy_date(islamic_date);
    calendar_cleanup();
    
    return 0;
}
```

#### Compile and Link

```bash
# Compile with the calendar library
gcc -o my_program my_program.c -lcalendar_lib -lm

# Or with static linking
gcc -o my_program my_program.c /usr/local/lib/libcalendar_lib.a -lm
```

### Integration with Other Systems

#### Shell Scripting

```bash
#!/bin/bash
# Daily calendar report script

echo "=== DAILY CALENDAR REPORT ==="
echo "Generated: $(date)"
echo ""

# Get today's conversions
calendar --today --format json > today.json

# Extract information using jq
GREGORIAN=$(jq -r '.gregorian.formatted' today.json)
ISLAMIC=$(jq -r '.islamic.formatted' today.json)
LUNAR_PHASE=$(jq -r '.lunar.phase' today.json)

echo "Gregorian: $GREGORIAN"
echo "Islamic: $ISLAMIC"
echo "Moon Phase: $LUNAR_PHASE"

# Get upcoming holidays
calendar --holidays --days 7 --format csv > upcoming.csv
HOLIDAY_COUNT=$(tail -n +2 upcoming.csv | wc -l)

echo ""
echo "Upcoming holidays (7 days): $HOLIDAY_COUNT"

# Clean up
rm today.json upcoming.csv
```

#### Python Integration

```python
#!/usr/bin/env python3
import subprocess
import json

def get_calendar_data(date_str):
    """Get calendar data for a specific date."""
    cmd = ['calendar', '--convert', '--date', date_str, '--format', 'json']
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        return json.loads(result.stdout)
    else:
        raise Exception(f"Calendar command failed: {result.stderr}")

def main():
    # Get data for today
    data = get_calendar_data('2025-06-17')
    
    print(f"Gregorian: {data['gregorian']['formatted']}")
    print(f"Islamic: {data['islamic']['formatted']}")
    print(f"Lunar Phase: {data['lunar']['phase']}")
    
    # Get holidays
    cmd = ['calendar', '--holidays', '--date', '2025-06-17', '--format', 'json']
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        holidays = json.loads(result.stdout)
        if holidays:
            print("\nHolidays today:")
            for holiday in holidays:
                print(f"- {holiday['name']} ({holiday['culture']})")
        else:
            print("\nNo holidays today")

if __name__ == "__main__":
    main()
```

## Troubleshooting

### Common Issues and Solutions

#### Installation Issues

**Problem**: `calendar: command not found`

**Solutions:**
```bash
# Check if installed
which calendar

# Check PATH
echo $PATH | grep -o '/usr/local/bin'

# Add to PATH if missing
echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# Reinstall if necessary
sudo ./install.sh
```

**Problem**: Permission denied when running

**Solutions:**
```bash
# Check permissions
ls -la /usr/local/bin/calendar

# Fix permissions
sudo chmod 755 /usr/local/bin/calendar
sudo chown root:wheel /usr/local/bin/calendar
```

#### Date Conversion Issues

**Problem**: Incorrect Islamic date conversions

**Cause**: Different Islamic calendar calculation methods

**Solutions:**
```bash
# Use astronomical calculation (default)
calendar --islamic-method astronomical

# Use civil calculation
calendar --islamic-method civil

# Check calculation method
calendar --show-methods
```

**Problem**: Chinese calendar showing wrong zodiac

**Cause**: Different New Year calculation

**Solutions:**
```bash
# Verify Chinese New Year date
calendar --chinese-new-year 2025

# Check zodiac calculation
calendar --chinese --zodiac --year 2025 --verbose
```

#### Lunar Phase Issues

**Problem**: Lunar phase times seem off

**Cause**: Timezone differences

**Solutions:**
```bash
# Set specific timezone
calendar --timezone UTC --moon

# Show all timezone info
calendar --moon --show-timezone

# Convert to local time
calendar --moon --local-time
```

#### GUI Application Issues

**Problem**: GUI won't launch on macOS

**Solutions:**
```bash
# Remove quarantine attribute
xattr -dr com.apple.quarantine /Applications/calendar_gui.app

# Check system requirements
sw_vers  # Requires macOS 10.14+

# Launch from terminal for error messages
/Applications/calendar_gui.app/Contents/MacOS/calendar_gui
```

**Problem**: GUI displays incorrectly

**Solutions:**
```bash
# Reset preferences
rm ~/Library/Preferences/com.calendar.multicalendar.plist

# Clear application state
rm -rf ~/Library/Application\ Support/Multi-Calendar\ System/

# Restart application
```

#### Export Issues

**Problem**: CSV export contains corrupted characters

**Cause**: Encoding issues

**Solutions:**
```bash
# Specify UTF-8 encoding
calendar --export csv --encoding utf-8 --file output.csv

# Use different format
calendar --export json --file output.json
```

**Problem**: iCal export not compatible with calendar app

**Cause**: Non-standard formatting

**Solutions:**
```bash
# Use standard iCal format
calendar --export ical --standard --file output.ics

# Validate iCal format
calendar --validate-ical output.ics
```

### Performance Issues

#### Slow Date Conversions

**Problem**: Large date range conversions are slow

**Solutions:**
```bash
# Use batch mode for better performance
calendar --batch-convert --input dates.txt --output results.csv

# Enable caching
export CALENDAR_ENABLE_CACHE=1

# Use parallel processing
calendar --parallel --threads 4 --batch-convert
```

#### Memory Usage

**Problem**: High memory usage with large exports

**Solutions:**
```bash
# Use streaming export
calendar --export csv --stream --file large_export.csv

# Process in chunks
calendar --export csv --chunk-size 1000 --file chunked_export.csv
```

### Data Issues

#### Missing Holiday Data

**Problem**: Some holidays not showing

**Solutions:**
```bash
# Update holiday database
calendar --update-holidays

# Check holiday database location
calendar --show-data-path

# Manually refresh data
rm -rf /usr/local/share/calendar/data/
calendar --init-data
```

#### Incorrect Calculations

**Problem**: Astronomical calculations seem wrong

**Solutions:**
```bash
# Verify calculation method
calendar --show-algorithms

# Compare with reference
calendar --validate --date 2025-06-17

# Enable debug mode
calendar --debug --moon --date 2025-06-17
```

### Getting Additional Help

#### Verbose Output

```bash
# Enable detailed logging
calendar --verbose --today

# Debug mode for troubleshooting
calendar --debug --convert --date 2025-06-17
```

#### System Information

```bash
# Show system and version info
calendar --system-info

# Output:
# Multi-Calendar System v2.0.0
# Built: 2025-06-17 03:22:17 UTC
# Platform: Darwin 21.6.0
# Architecture: x86_64
# Compiler: Apple clang version 14.0.0
# Libraries: libcalendar_lib.a
# Data Path: /usr/local/share/calendar/
# Config: ~/.calendar-config
```

#### Diagnostic Mode

```bash
# Run comprehensive diagnostics
calendar --diagnostics

# Test all calendar systems
calendar --test-all

# Validate installation
calendar --validate-install
```

#### Support Resources

1. **Documentation**: Check `/usr/local/share/calendar/docs/`
2. **Log Files**: Check `~/.calendar-system/logs/`
3. **GitHub Issues**: Report bugs at repository issues page
4. **Community Forum**: Join discussions for help
5. **Email Support**: Contact maintainers for critical issues

### Performance Tuning

#### Optimization Settings

```bash
# Enable performance optimizations
export CALENDAR_OPTIMIZE=1
export CALENDAR_CACHE_SIZE=1000
export CALENDAR_PARALLEL_THREADS=4

# Benchmark performance
calendar --benchmark --operations 1000
```

#### Memory Management

```bash
# Monitor memory usage
calendar --monitor-memory --export csv --range 2025-01-01:2025-12-31

# Use memory-efficient mode
calendar --low-memory --export csv --stream
```

---

*Usage guide last updated: 2025-06-17 03:22:17 UTC*
*Author: Linh Duong Tuan*