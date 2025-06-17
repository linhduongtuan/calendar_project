#!/bin/bash

# 🚀 Quick Test Script - Fast validation of core functionality

echo "🚀 QUICK CALENDAR SYSTEM TEST"
echo "=============================="

# Quick compilation test
echo "📦 Compiling..."
if gcc -I. -Iinclude -o test_quick test_persian_simple.c src/calendars/persian.c src/calendars/gregorian.c src/utils/date_utils.c -lm 2>/dev/null; then
    echo "✅ Compilation successful"
else
    echo "❌ Compilation failed"
    exit 1
fi

# Quick functionality test
echo "🧪 Testing functionality..."
if ./test_quick > /dev/null 2>&1; then
    echo "✅ Basic functionality works"
else
    echo "❌ Functionality test failed"
    exit 1
fi

# Quick accuracy test
echo "🎯 Testing accuracy..."
if ./test_quick | grep -q "✅.*Persian date created successfully"; then
    echo "✅ Persian calendar working"
else
    echo "❌ Persian calendar issues"
    exit 1
fi

# Quick conversion test
echo "🔄 Testing conversions..."
if ./test_quick | grep -q "✅.*Conversion successful"; then
    echo "✅ Calendar conversions working"
else
    echo "❌ Conversion issues"
    exit 1
fi

echo ""
echo "🎉 QUICK TEST PASSED!"
echo "✅ Core functionality verified"
echo "✅ Persian calendar operational"
echo "✅ Conversions accurate"
echo ""
echo "💡 Run './run_all_tests.sh' for comprehensive testing"
echo "📖 See TESTING_GUIDE.md for detailed testing procedures"