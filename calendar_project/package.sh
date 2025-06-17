# package.sh - Fixed DMG creation for macOS
#!/bin/bash

VERSION="2.0.0"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "======================================"
echo "Multi-Calendar System Package Creator"
echo "======================================"

# Create package directory
PACKAGE_DIR="multi-calendar-system-$VERSION"
echo "Creating package directory: $PACKAGE_DIR"

# Clean up any existing package directory
rm -rf "$PACKAGE_DIR"
mkdir -p "$PACKAGE_DIR"

# Copy built binaries
if [[ -d "build" ]]; then
    echo "Copying build files..."
    cp -r build/ "$PACKAGE_DIR/"
else
    echo "❌ Error: build/ directory not found. Please compile first."
    exit 1
fi

# Copy source for reference
echo "Copying source files..."
mkdir -p "$PACKAGE_DIR/src"
if [[ -d "src" ]]; then
    cp -r src/ "$PACKAGE_DIR/src/"
fi
if [[ -d "include" ]]; then
    cp -r include/ "$PACKAGE_DIR/include/"
fi

# Copy data files
echo "Copying data files..."
if [[ -d "data" ]]; then
    cp -r data/ "$PACKAGE_DIR/"
else
    mkdir -p "$PACKAGE_DIR/data"
    echo "# Sample holiday data will be created on first run" > "$PACKAGE_DIR/data/README.md"
fi

# Copy documentation
echo "Creating documentation..."
mkdir -p "$PACKAGE_DIR/docs"

# Create README
cat > "$PACKAGE_DIR/README.md" << 'EOF'
# Multi-Calendar System v2.0.0

A comprehensive calendar system supporting Gregorian, Islamic, Chinese, and Hebrew calendars with astronomical calculations.

## Features
- ✅ Multiple calendar systems with accurate conversions
- 🌙 Lunar phase calculations and astronomical data
- 🎉 Holiday database with cultural celebrations
- 📱 Native macOS GUI application
- 🖥️ Command-line interface for all platforms
- 📁 Export functionality (CSV, iCal)
- 🕰️ Timezone support

## Installation

### Quick Install
```bash
sudo ./install.sh