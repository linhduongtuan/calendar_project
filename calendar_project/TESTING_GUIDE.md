# 🧪 Testing Guide - Multi-Calendar System Extensions

## 📋 Overview

This guide provides comprehensive testing strategies for validating the calendar system extensions, ensuring accuracy, reliability, and cultural authenticity.

## 🎯 Testing Levels

### 1. **Unit Testing** - Individual Components
### 2. **Integration Testing** - Component Interactions  
### 3. **Accuracy Testing** - Mathematical Precision
### 4. **Cultural Testing** - Authenticity Validation
### 5. **Performance Testing** - Speed and Memory
### 6. **User Acceptance Testing** - Real-world Usage

## 🔧 Testing Tools and Methods

### **Quick Manual Tests** (What we just did)
```bash
# Compile and run simple tests
cd /Users/linh/Downloads/calendar_project
gcc -I. -Iinclude -o test_persian test_persian_simple.c src/calendars/persian.c src/calendars/gregorian.c src/utils/date_utils.c -lm
./test_persian

# Run comprehensive demo
gcc -I. -Iinclude -o demo_extensions demo_extensions.c src/calendars/persian.c src/calendars/gregorian.c src/calendars/islamic.c src/utils/date_utils.c -lm
./demo_extensions
```

### **Automated Unit Tests** (Professional approach)
```bash
# Build with testing enabled
mkdir build_test && cd build_test
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -f ../CMakeLists_extended.txt ..
make
make test  # Run all unit tests
```

### **Reference Validation** (Accuracy verification)
```bash
# Compare against known reference implementations
./test_accuracy_validation
```

## 🧪 Specific Test Categories

### **1. Persian Calendar Tests**

#### **A. Basic Functionality Tests**
```c
// Test date creation
PersianDate* date = persian_create_date(1, 1, 1404);
assert(date != NULL);
assert(date->base.day == 1);
assert(date->base.month == 1);
assert(date->base.year == 1404);

// Test validation
assert(persian_validate_date(32, 1, 1404) == CALENDAR_ERROR_INVALID_DATE);
assert(persian_validate_date(1, 13, 1404) == CALENDAR_ERROR_INVALID_DATE);
assert(persian_validate_date(30, 12, 1404) == CALENDAR_ERROR_INVALID_DATE); // Non-leap year
```

#### **B. Leap Year Tests**
```c
// Test known leap years in Persian calendar
assert(persian_is_leap_year(1403) == 1);  // Leap year
assert(persian_is_leap_year(1404) == 0);  // Regular year
assert(persian_is_leap_year(1407) == 1);  // Leap year
assert(persian_is_leap_year(1408) == 0);  // Regular year
```

#### **C. Conversion Accuracy Tests**
```c
// Test known conversion points
GregorianDate* nowruz_greg = gregorian_create_date(21, 3, 2025);
PersianDate persian_result;
persian_from_gregorian(nowruz_greg, &persian_result);

// Nowruz should be 1 Farvardin
assert(persian_result.base.day == 1);
assert(persian_result.base.month == 1);
assert(persian_result.base.year == 1404);
```

### **2. Astronomical Tests**

#### **A. Lunar Calculation Tests**
```c
// Test moon phase calculations
double moon_age = lunar_calculate_moon_age(2460844); // June 17, 2025
LunarPhase phase = lunar_get_phase(moon_age);
double illumination = lunar_calculate_illumination(moon_age);

assert(moon_age >= 0 && moon_age <= 29.53);
assert(illumination >= 0.0 && illumination <= 1.0);
```

#### **B. Solar Calculation Tests**
```c
// Test solar position calculations
SolarPosition* pos = solar_calculate_position(2460844.5);
assert(pos != NULL);
assert(pos->solar_longitude >= 0 && pos->solar_longitude < 360);
assert(pos->declination >= -23.5 && pos->declination <= 23.5);
```

### **3. Prayer Times Tests**

#### **A. Known Location Tests**
```c
// Test Mecca prayer times (known reference)
PrayerTimes* times = prayer_calculate_times(
    2460844.5,  // Julian day
    21.4225,    // Mecca latitude
    39.8262,    // Mecca longitude
    PRAYER_METHOD_MAKKAH,
    3           // UTC+3
);

assert(times != NULL);
assert(times->dhuhr_jd > times->fajr_jd);
assert(times->asr_jd > times->dhuhr_jd);
assert(times->maghrib_jd > times->asr_jd);
assert(times->isha_jd > times->maghrib_jd);
```

#### **B. Qibla Direction Tests**
```c
// Test Qibla from known locations
QiblaDirection* qibla = prayer_calculate_qibla(40.7128, -74.0060); // New York
assert(qibla != NULL);
assert(qibla->qibla_bearing >= 0 && qibla->qibla_bearing < 360);
// New York to Mecca should be approximately 58° (northeast)
assert(qibla->qibla_bearing > 50 && qibla->qibla_bearing < 70);
```

## 📊 Reference Data for Validation

### **Persian Calendar Reference Points**
```
Known Accurate Conversions:
- Persian 1/1/1404 = Gregorian March 21/22, 2025 (Nowruz)
- Persian 1/1/1403 = Gregorian March 21, 2024 (Leap year)
- Persian 1/7/1404 = Gregorian September 23/24, 2025 (Autumn equinox)

Leap Year Pattern (33-year cycle):
Years 1, 5, 9, 13, 17, 22, 26, 30 of each cycle are leap years
```

### **Astronomical Reference Points**
```
Known Values for June 17, 2025:
- Moon age: ~18-20 days (Waning Gibbous)
- Solar longitude: ~86° (late Gemini)
- Sunrise in London: ~04:43 UTC
- Sunset in London: ~20:21 UTC
```

### **Prayer Times Reference**
```
Mecca, June 17, 2025 (approximate):
- Fajr: 04:15 local time
- Dhuhr: 12:18 local time  
- Asr: 15:42 local time
- Maghrib: 18:54 local time
- Isha: 20:24 local time
```

## 🎯 Automated Testing Scripts

### **Create Comprehensive Test Suite**
```bash
# Create test runner script
cat > run_all_tests.sh << 'EOF'
#!/bin/bash
echo "🧪 Running Calendar System Test Suite"
echo "====================================="

# Compile all tests
echo "📦 Compiling tests..."
gcc -I. -Iinclude -o test_persian_unit test_persian_unit.c src/calendars/persian.c src/calendars/gregorian.c src/utils/date_utils.c -lm
gcc -I. -Iinclude -o test_conversions test_conversions.c src/calendars/*.c src/utils/date_utils.c -lm
gcc -I. -Iinclude -o test_accuracy test_accuracy.c src/calendars/*.c src/utils/date_utils.c -lm

# Run tests
echo "🏃 Running unit tests..."
./test_persian_unit
echo "🔄 Running conversion tests..."
./test_conversions  
echo "🎯 Running accuracy tests..."
./test_accuracy

echo "✅ All tests completed!"
EOF

chmod +x run_all_tests.sh
```

### **Continuous Integration Test**
```yaml
# .github/workflows/test.yml
name: Calendar System Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: sudo apt-get install build-essential cmake
    - name: Build and test
      run: |
        mkdir build && cd build
        cmake -DBUILD_TESTS=ON ..
        make
        make test
```

## 🔍 Manual Testing Procedures

### **1. Visual Inspection Tests**
```bash
# Test calendar display
./calendar_extended
# Choose option 1: Calendar Conversions
# Verify dates look reasonable and culturally accurate
```

### **2. Cross-Reference Testing**
```bash
# Compare with online converters
# Persian: https://www.fourmilab.ch/documents/calendar/
# Islamic: https://www.islamicfinder.org/islamic-date-converter/
# Astronomical: https://www.timeanddate.com/astronomy/
```

### **3. Edge Case Testing**
```bash
# Test boundary conditions
- Year 1 Persian calendar
- Year 9999 Persian calendar  
- February 29 leap years
- Last day of Persian months
- Prayer times at extreme latitudes
- Solar calculations at solstices/equinoxes
```

## 📈 Performance Testing

### **Speed Tests**
```c
#include <time.h>

// Benchmark conversion speed
clock_t start = clock();
for (int i = 0; i < 100000; i++) {
    PersianDate* date = persian_create_date(1, 1, 1404);
    GregorianDate greg;
    persian_to_gregorian(date, &greg);
    persian_destroy_date(date);
}
clock_t end = clock();
double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
printf("100,000 conversions in %.2f seconds\n", cpu_time);
```

### **Memory Tests**
```bash
# Check for memory leaks
valgrind --leak-check=full ./test_persian
valgrind --leak-check=full ./demo_extensions
```

## 🌍 Cultural Accuracy Testing

### **Persian Calendar Cultural Tests**
- ✅ Nowruz falls on spring equinox (March 20-21)
- ✅ Month names are authentic Persian transliterations
- ✅ Seasonal divisions align with Persian tradition
- ✅ Leap year pattern follows astronomical observations

### **Islamic Calendar Cultural Tests**  
- ✅ Month names are correct Arabic transliterations
- ✅ Epoch starts from Hijra (622 CE)
- ✅ Purely lunar calendar (no solar adjustment)

### **Prayer Times Cultural Tests**
- ✅ Multiple calculation methods supported
- ✅ Qibla direction points toward Kaaba
- ✅ High latitude adjustments available
- ✅ Times align with Islamic jurisprudence

## 🚨 Error Testing

### **Input Validation Tests**
```c
// Test invalid inputs
assert(persian_create_date(0, 1, 1404) == NULL);      // Invalid day
assert(persian_create_date(1, 0, 1404) == NULL);      // Invalid month  
assert(persian_create_date(1, 1, 0) == NULL);         // Invalid year
assert(persian_create_date(30, 12, 1404) == NULL);    // Invalid day for month
```

### **Null Pointer Tests**
```c
// Test null pointer handling
assert(persian_to_gregorian(NULL, &greg) == CALENDAR_ERROR_NULL_POINTER);
assert(persian_from_gregorian(&greg, NULL) == CALENDAR_ERROR_NULL_POINTER);
```

## 📋 Test Checklist

### **Before Release**
- [ ] All unit tests pass
- [ ] Conversion accuracy within ±1 day
- [ ] No memory leaks detected
- [ ] Performance meets requirements
- [ ] Cultural accuracy verified
- [ ] Edge cases handled properly
- [ ] Documentation updated
- [ ] Cross-platform compatibility confirmed

### **Regression Testing**
- [ ] Existing functionality unchanged
- [ ] New features don't break old code
- [ ] API compatibility maintained
- [ ] File format compatibility preserved

## 🎯 Testing Best Practices

### **1. Test-Driven Development**
- Write tests before implementing features
- Use tests to define expected behavior
- Refactor with confidence knowing tests will catch regressions

### **2. Comprehensive Coverage**
- Test normal cases, edge cases, and error cases
- Include boundary value testing
- Test with real-world data

### **3. Cultural Sensitivity**
- Validate against authoritative sources
- Consult with cultural experts when possible
- Test with native speakers/practitioners

### **4. Continuous Testing**
- Run tests automatically on code changes
- Include performance regression testing
- Monitor accuracy over time

## 🔧 Quick Test Commands

```bash
# Quick functionality test
./test_persian && echo "✅ Persian calendar working"

# Quick accuracy test  
./demo_extensions | grep "Accuracy: Perfect" && echo "✅ Conversions accurate"

# Quick memory test
valgrind --leak-check=yes --quiet ./test_persian 2>&1 | grep "no leaks" && echo "✅ No memory leaks"

# Quick performance test
time ./demo_extensions > /dev/null && echo "✅ Performance acceptable"
```

This comprehensive testing approach ensures your calendar system extensions are reliable, accurate, and culturally authentic!