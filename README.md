# 🗓️ Multi-Calendar System

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/linhduongtuan/multi-calendar-system/releases)
[![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Unix-lightgrey.svg)](#supported-platforms)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#installation)

A comprehensive calendar system supporting multiple calendar formats with astronomical calculations, cultural holidays, and native GUI interface.

![Multi-Calendar System Screenshot](docs/images/calendar-gui-preview.png)

## ✨ Features

### 📅 **Multiple Calendar Systems**
- **Gregorian Calendar** - Standard Western calendar with accurate leap year calculations
- **Islamic Calendar (Hijri)** - Lunar-based calendar with precise month calculations
- **Chinese Calendar** - Traditional calendar with zodiac years and cycle tracking
- **Hebrew Calendar** - Jewish calendar with Metonic cycle accuracy

### 🌙 **Astronomical Calculations**
- **Lunar Phases** - New moon, full moon, first/last quarter tracking
- **Moon Age** - Days since new moon with precise calculations
- **Illumination** - Percentage of moon surface visible
- **Astronomical Events** - Solar/lunar eclipse predictions (future enhancement)

### 🎉 **Cultural Features**
- **Holiday Database** - Major celebrations from multiple cultures
- **Religious Observances** - Multi-faith calendar integration
- **Regional Events** - Country-specific holidays and celebrations
- **Custom Events** - Add your own important dates

### 📱 **User Interfaces**
- **Native macOS GUI** - Beautiful 3-tab interface with real-time updates
- **Cross-platform CLI** - Interactive terminal interface for all systems
- **Export Functionality** - CSV, iCal, and JSON format support
- **Timezone Support** - Global timezone handling and conversions

## 🚀 Quick Start

### macOS Users (Recommended)

#### Option 1: Professional Installer (.pkg)
```bash
# Download multi-calendar-system-2.0.0.pkg
# Double-click to run installer wizard
# Launch from Applications or type 'calendar' in Terminal
```

#### Option 2: Drag-and-Drop (.dmg)
```bash
# Download multi-calendar-system-2.0.0.dmg
# Mount DMG and drag app to Applications folder
# Optional: Run "Install CLI Tools.command" for terminal access
```

### Linux/Unix Users

```bash
# Download and extract
wget https://github.com/linhduongtuan/multi-calendar-system/releases/download/v2.0.0/multi-calendar-system-2.0.0.tar.gz
tar -xzf multi-calendar-system-2.0.0.tar.gz
cd multi-calendar-system-2.0.0

# Install
sudo ./install.sh

# Run
calendar
```

### Windows Users (WSL)

```bash
# Download ZIP package
# Extract multi-calendar-system-2.0.0.zip
# Follow Linux installation steps in WSL environment
```

## 📦 Installation Options

### Automatic Installation
```bash
# Full installation (CLI + GUI on macOS)
sudo ./install.sh

# Choose from:
# 1) Full installation (CLI + GUI)
# 2) Command-line only
# 3) GUI only (macOS)
```

### Manual Installation
```bash
# CLI tools
sudo cp build/calendar /usr/local/bin/
sudo cp -r data/ /usr/local/share/calendar/

# GUI app (macOS only)
sudo cp -r build/calendar_gui.app /Applications/
```

### Build from Source
```bash
git clone https://github.com/linhduongtuan/multi-calendar-system.git
cd multi-calendar-system
mkdir build && cd build

cmake -DBUILD_GUI=ON \
      -DBUILD_ASTRONOMY=ON \
      -DBUILD_HOLIDAYS=ON \
      -DBUILD_TIMEZONE=ON \
      -DBUILD_EXPORT=ON ..

make
sudo make install
```

## 💻 Usage

### Command-Line Interface

```bash
# Interactive menu
calendar

# Direct commands
calendar --today           # Show today in all calendars
calendar --convert         # Convert between calendars
calendar --moon           # Show current moon phase
calendar --holidays       # Display upcoming holidays
calendar --export csv     # Export data to CSV

# Get help
calendar --help
calendar --version
```

#### Example CLI Output
```
=== MULTI-CALENDAR SYSTEM v2.0.0 ===

Today's Date: 2025-06-17 03:13:36 UTC

📅 CALENDAR CONVERSIONS:
Gregorian: Tuesday, June 17, 2025
Islamic:   20 Dhu al-Hijjah 1446 AH  
Chinese:   Year of Snake (Yi-Si), Month 5, Day 23
Hebrew:    22 Sivan 5785

🌙 LUNAR INFORMATION:
Phase:        Waning Gibbous
Age:          18.7 days
Illumination: 67.3%
Next New:     June 25, 2025

🎉 UPCOMING HOLIDAYS:
June 21: Summer Solstice
July 4:  Independence Day (US)
July 17: Muharram 1447 AH (Islamic New Year)
```

### GUI Interface (macOS)

Launch from Applications folder or run:
```bash
open /Applications/calendar_gui.app
```

#### GUI Features:
- **📅 Calendar Tab**: Live date conversions and calculations
- **🎉 Holidays Tab**: Cultural celebrations browser
- **📁 Export Tab**: Data export with format options

![GUI Interface](docs/images/gui-interface.png)

## 🌍 Supported Platforms

| Platform | CLI Support | GUI Support | Installation |
|----------|-------------|-------------|--------------|
| **macOS 10.14+** | ✅ Full | ✅ Native Cocoa | PKG, DMG, or source |
| **Linux** | ✅ Full | ❌ | tar.gz or source |
| **Unix** | ✅ Full | ❌ | tar.gz or source |
| **Windows** | ⚠️ WSL only | ❌ | ZIP via WSL |

## 📚 Documentation

### Calendar Systems

#### Gregorian Calendar
- **Accuracy**: Proleptic Gregorian with precise leap year rules
- **Range**: 1 CE to 9999 CE
- **Features**: Day-of-week calculation using Zeller's congruence

#### Islamic Calendar (Hijri)
- **Accuracy**: Astronomical calculation based
- **Epoch**: July 16, 622 CE (1 Muharram 1 AH)
- **Features**: Precise lunar month tracking

#### Chinese Calendar
- **System**: Traditional lunisolar calendar
- **Features**: Zodiac animals, heavenly stems, earthly branches
- **Cycles**: 60-year cycle tracking

#### Hebrew Calendar
- **Accuracy**: Metonic cycle based (19-year cycle)
- **Features**: Religious observance integration
- **Molad**: New moon calculation accuracy

### Astronomical Calculations

```bash
# Moon phase algorithm
lunar_age = (julian_day - known_new_moon) % 29.53058867

# Illumination calculation  
illumination = (1 - cos(lunar_age * 2π / 29.53058867)) / 2 * 100
```

### Holiday Database Structure

```json
{
  "holidays": [
    {
      "name": "New Year's Day",
      "date": "2025-01-01", 
      "type": "secular",
      "countries": ["US", "UK", "CA"],
      "description": "Beginning of the Gregorian calendar year"
    }
  ]
}
```

## 🛠️ Development

### Building Requirements

- **CMake** 3.10+
- **C Compiler** (GCC, Clang, or MSVC)
- **macOS**: Xcode Command Line Tools
- **Linux**: build-essential package

### Build Configuration

```bash
# Full build with all features
cmake -DBUILD_GUI=ON \
      -DBUILD_ASTRONOMY=ON \
      -DBUILD_HOLIDAYS=ON \
      -DBUILD_TIMEZONE=ON \
      -DBUILD_EXPORT=ON ..

# Minimal CLI-only build
cmake -DBUILD_GUI=OFF \
      -DBUILD_ASTRONOMY=OFF \
      -DBUILD_HOLIDAYS=OFF \
      -DBUILD_TIMEZONE=OFF \
      -DBUILD_EXPORT=OFF ..
```

### Project Structure

```
multi-calendar-system/
├── src/
│   ├── calendars/          # Calendar system implementations
│   ├── astronomy/          # Lunar calculations
│   ├── holidays/           # Holiday database
│   ├── gui/               # macOS GUI (Objective-C)
│   ├── utils/             # Utility functions
│   └── main.c             # CLI entry point
├── include/               # Header files
├── data/                  # Holiday and configuration data
├── docs/                  # Documentation
├── build/                 # Build output
└── CMakeLists.txt        # Build configuration
```

### Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Make your changes with tests
4. Commit: `git commit -m "Add feature description"`
5. Push: `git push origin feature-name`
6. Submit a Pull Request

## 🐛 Troubleshooting

### Common Issues

#### "Command not found: calendar"
```bash
# Check if installed
which calendar

# Add to PATH
echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

#### GUI app won't launch (macOS)
```bash
# Remove quarantine attribute
xattr -dr com.apple.quarantine /Applications/calendar_gui.app

# Check system requirements
sw_vers  # Requires macOS 10.14+
```

#### Permission denied
```bash
# Fix installation permissions
sudo chmod 755 /usr/local/bin/calendar
sudo chown root:wheel /usr/local/bin/calendar
```

#### Build errors
```bash
# Install dependencies (macOS)
xcode-select --install

# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential cmake

# Clean build
rm -rf build && mkdir build && cd build
```

### Getting Help

- **📖 Documentation**: Check the `docs/` directory
- **🐛 Bug Reports**: [Open an issue](https://github.com/linhduongtuan/multi-calendar-system/issues)
- **💡 Feature Requests**: [Submit enhancement](https://github.com/linhduongtuan/multi-calendar-system/issues/new)
- **💬 Discussions**: [Community forum](https://github.com/linhduongtuan/multi-calendar-system/discussions)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Linh Duong Tuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 Acknowledgments

- **Astronomical Algorithms** by Jean Meeus for lunar calculations
- **Calendar Conversion Algorithms** by Edward M. Reingold and Nachum Dershowitz
- **Islamic Calendar** calculations based on astronomical observations
- **Hebrew Calendar** implementation following traditional Metonic cycles
- **macOS GUI** developed with native Cocoa frameworks

## 📊 Project Stats

- **⭐ Stars**: ![GitHub stars](https://img.shields.io/github/stars/linhduongtuan/multi-calendar-system?style=social)
- **🍴 Forks**: ![GitHub forks](https://img.shields.io/github/forks/linhduongtuan/multi-calendar-system?style=social)
- **📥 Downloads**: ![GitHub downloads](https://img.shields.io/github/downloads/linhduongtuan/multi-calendar-system/total)
- **🐛 Issues**: ![GitHub issues](https://img.shields.io/github/issues/linhduongtuan/multi-calendar-system)

---

**Built with ❤️ by [Linh Duong Tuan](https://github.com/linhduongtuan)**

*Last updated: 2025-06-17 03:13:36 UTC*

**🔗 Quick Links:**
- [📦 Download Latest Release](https://github.com/linhduongtuan/multi-calendar-system/releases/latest)
- [📖 Full Documentation](docs/)
- [🚀 Getting Started Guide](docs/getting-started.md)
- [🛠️ Development Guide](docs/development.md)
- [📝 Changelog](CHANGELOG.md)
