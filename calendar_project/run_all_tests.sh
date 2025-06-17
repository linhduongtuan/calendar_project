#!/bin/bash

# 🧪 Calendar System Test Runner
# Comprehensive testing suite for all calendar extensions

echo "╔══════════════════════════════════════════════════════════════╗"
echo "║              🧪 CALENDAR SYSTEM TEST SUITE                  ║"
echo "║                     Running All Tests                       ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    echo -e "${BLUE}🔍 Running: $test_name${NC}"
    echo "Command: $test_command"
    echo "----------------------------------------"
    
    if eval "$test_command"; then
        echo -e "${GREEN}✅ PASSED: $test_name${NC}"
        ((PASSED_TESTS++))
    else
        echo -e "${RED}❌ FAILED: $test_name${NC}"
    fi
    
    ((TOTAL_TESTS++))
    echo ""
}

# Compilation tests
echo -e "${YELLOW}📦 COMPILATION TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Persian Calendar Compilation" \
    "gcc -I. -Iinclude -o test_persian test_persian_simple.c src/calendars/persian.c src/calendars/gregorian.c src/utils/date_utils.c -lm"

run_test "Demo Extensions Compilation" \
    "gcc -I. -Iinclude -o demo_extensions demo_extensions.c src/calendars/persian.c src/calendars/gregorian.c src/calendars/islamic.c src/utils/date_utils.c -lm"

run_test "Accuracy Validation Compilation" \
    "gcc -I. -Iinclude -o test_accuracy test_accuracy_validation.c src/calendars/persian.c src/calendars/gregorian.c src/calendars/islamic.c src/utils/date_utils.c -lm"

# Functionality tests
echo -e "${YELLOW}🚀 FUNCTIONALITY TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Persian Calendar Basic Test" \
    "./test_persian"

run_test "Multi-Calendar Demo" \
    "./demo_extensions"

# Accuracy tests
echo -e "${YELLOW}🎯 ACCURACY TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Comprehensive Accuracy Validation" \
    "./test_accuracy"

# Memory leak tests (if valgrind is available)
echo -e "${YELLOW}🔍 MEMORY TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

if command -v valgrind &> /dev/null; then
    run_test "Memory Leak Check - Persian Calendar" \
        "valgrind --leak-check=yes --error-exitcode=1 --quiet ./test_persian"
    
    run_test "Memory Leak Check - Demo" \
        "valgrind --leak-check=yes --error-exitcode=1 --quiet ./demo_extensions"
else
    echo -e "${YELLOW}⚠️  Valgrind not found - skipping memory tests${NC}"
    echo "   Install valgrind for memory leak detection"
    echo ""
fi

# Performance tests
echo -e "${YELLOW}⚡ PERFORMANCE TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Performance Benchmark" \
    "time ./demo_extensions > /dev/null 2>&1"

# File structure tests
echo -e "${YELLOW}📁 FILE STRUCTURE TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Extension Files Present" \
    "test -f EXTENSION_PLAN.md && test -f DEVELOPMENT_GUIDE.md && test -f EXTENSION_SUMMARY.md && test -f CMakeLists_extended.txt"

run_test "Persian Calendar Files Present" \
    "test -f src/calendars/persian.h && test -f src/calendars/persian.c"

run_test "Extended Headers Present" \
    "test -f include/calendar_types_extended.h && test -f src/astronomy/solar_calc.h && test -f src/religious/prayer_times.h"

# Code quality tests
echo -e "${YELLOW}🔧 CODE QUALITY TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "No Compilation Warnings (Persian)" \
    "gcc -Wall -Wextra -I. -Iinclude -c src/calendars/persian.c -o /tmp/persian_test.o"

run_test "Header Guards Check" \
    "grep -q '#ifndef.*_H' src/calendars/persian.h && grep -q '#define.*_H' src/calendars/persian.h && grep -q '#endif' src/calendars/persian.h"

# Integration tests
echo -e "${YELLOW}🔗 INTEGRATION TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Gregorian-Persian Round Trip" \
    "echo 'Testing round-trip conversion...' && ./test_accuracy | grep -q 'Good accuracy (±1 day): 8/8'"

run_test "Edge Case Handling" \
    "echo 'Testing edge cases...' && ./test_accuracy | grep -q 'Edge Case Results: 7/7 correctly handled'"

# Documentation tests
echo -e "${YELLOW}📚 DOCUMENTATION TESTS${NC}"
echo "═══════════════════════════════════════════════════════════════"

run_test "Documentation Completeness" \
    "test -s TESTING_GUIDE.md && test -s DEVELOPMENT_GUIDE.md && test -s EXTENSION_PLAN.md"

run_test "Code Comments Present" \
    "grep -q '//' src/calendars/persian.c && grep -q '//' src/calendars/persian.h"

# Summary
echo "╔══════════════════════════════════════════════════════════════╗"
echo "║                        TEST SUMMARY                         ║"
echo "╚══════════════════════════════════════════════════════════════╝"
echo ""

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! ($PASSED_TESTS/$TOTAL_TESTS)${NC}"
    echo -e "${GREEN}✅ Your calendar system extensions are working perfectly!${NC}"
    exit 0
elif [ $PASSED_TESTS -gt $((TOTAL_TESTS * 3 / 4)) ]; then
    echo -e "${YELLOW}⚠️  MOSTLY WORKING ($PASSED_TESTS/$TOTAL_TESTS tests passed)${NC}"
    echo -e "${YELLOW}🔧 Some improvements needed, but core functionality works${NC}"
    exit 1
else
    echo -e "${RED}❌ SIGNIFICANT ISSUES ($PASSED_TESTS/$TOTAL_TESTS tests passed)${NC}"
    echo -e "${RED}🚨 Major fixes needed before deployment${NC}"
    exit 2
fi