# create_pkg.sh - Enhanced macOS installer package
#!/bin/bash

VERSION="2.0.0"
IDENTIFIER="com.calendar.multicalendar"
PKG_NAME="multi-calendar-system-$VERSION.pkg"

echo "======================================"
echo "Creating macOS Installer Package (.pkg)"
echo "======================================"

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ PKG creation only works on macOS"
    exit 1
fi

# Check required tools
if ! command -v pkgbuild &> /dev/null; then
    echo "❌ pkgbuild command not found"
    exit 1
fi

if ! command -v productbuild &> /dev/null; then
    echo "❌ productbuild command not found"
    exit 1
fi

echo "✅ macOS detected"
echo "✅ Package building tools available"

# Clean up any existing build directories
rm -rf pkg_build pkg_scripts pkg_resources *.pkg

echo ""
echo "📁 Creating package structure..."

# Create package root structure
mkdir -p pkg_build/usr/local/bin
mkdir -p pkg_build/usr/local/lib
mkdir -p pkg_build/usr/local/share/calendar
mkdir -p pkg_build/Applications

# Copy binaries
echo "📦 Copying binaries..."
if [[ -f "build/calendar" ]]; then
    cp build/calendar pkg_build/usr/local/bin/
    chmod 755 pkg_build/usr/local/bin/calendar
    echo "✅ CLI binary copied"
else
    echo "❌ CLI binary not found at build/calendar"
    exit 1
fi

if [[ -f "build/libcalendar_lib.a" ]]; then
    cp build/libcalendar_lib.a pkg_build/usr/local/lib/
    chmod 644 pkg_build/usr/local/lib/libcalendar_lib.a
    echo "✅ Library copied"
fi

# Copy GUI app
if [[ -d "build/calendar_gui.app" ]]; then
    cp -r build/calendar_gui.app pkg_build/Applications/
    chmod -R 755 pkg_build/Applications/calendar_gui.app
    echo "✅ GUI app copied"
else
    echo "⚠️  GUI app not found, creating CLI-only package"
fi

# Copy data files
echo "📄 Copying data files..."
if [[ -d "data" ]]; then
    cp -r data/* pkg_build/usr/local/share/calendar/ 2>/dev/null || mkdir -p pkg_build/usr/local/share/calendar/data
else
    mkdir -p pkg_build/usr/local/share/calendar/data
    echo "# Holiday data will be generated on first run" > pkg_build/usr/local/share/calendar/data/README.md
fi

# Create package scripts directory
mkdir -p pkg_scripts

# Create postinstall script
cat > pkg_scripts/postinstall << 'EOF'
#!/bin/bash

echo "Multi-Calendar System Post-Installation"

# Fix permissions
chown -R root:wheel /usr/local/bin/calendar
chown -R root:wheel /usr/local/lib/libcalendar_lib.a
chown -R root:wheel /usr/local/share/calendar/

if [[ -d "/Applications/calendar_gui.app" ]]; then
    chown -R root:wheel /Applications/calendar_gui.app
fi

# Add to PATH if needed
USER_HOME=$(eval echo ~$USER)
SHELL_RC=""

# Detect shell and set appropriate RC file
if [[ "$SHELL" == *"zsh"* ]]; then
    SHELL_RC="$USER_HOME/.zshrc"
elif [[ "$SHELL" == *"bash"* ]]; then
    SHELL_RC="$USER_HOME/.bash_profile"
fi

# Add /usr/local/bin to PATH if not already there
if [[ -n "$SHELL_RC" && -f "$SHELL_RC" ]]; then
    if ! grep -q "/usr/local/bin" "$SHELL_RC"; then
        echo 'export PATH="/usr/local/bin:$PATH"' >> "$SHELL_RC"
        echo "Added /usr/local/bin to PATH in $SHELL_RC"
    fi
fi

# Create desktop shortcut
if [[ -d "$USER_HOME/Desktop" ]]; then
    cat > "$USER_HOME/Desktop/Multi-Calendar System.command" << 'EOFCMD'
#!/bin/bash
/usr/local/bin/calendar
EOFCMD
    chmod +x "$USER_HOME/Desktop/Multi-Calendar System.command"
    chown $USER:staff "$USER_HOME/Desktop/Multi-Calendar System.command"
fi

echo "Installation completed successfully!"
echo ""
echo "Usage:"
echo "  CLI: Open Terminal and type 'calendar'"
echo "  GUI: Launch from Applications folder"
echo ""
echo "Note: You may need to restart Terminal to use the 'calendar' command"

exit 0
EOF

# Create preinstall script (cleanup any previous installation)
cat > pkg_scripts/preinstall << 'EOF'
#!/bin/bash

echo "Preparing Multi-Calendar System installation..."

# Remove any previous installation
rm -f /usr/local/bin/calendar
rm -f /usr/local/lib/libcalendar_lib.a
rm -rf /usr/local/share/calendar/
rm -rf /Applications/calendar_gui.app

echo "Previous installation cleaned up"
exit 0
EOF

chmod +x pkg_scripts/*

# Create package resources
mkdir -p pkg_resources

# Create welcome message
cat > pkg_resources/Welcome.txt << 'EOF'
Welcome to Multi-Calendar System v2.0.0

This installer will install:

COMMAND-LINE INTERFACE:
• Multi-calendar date conversions
• Lunar phase calculations  
• Holiday database
• Export functionality

GUI APPLICATION (macOS):
• Native macOS interface
• 3-tab design (Calendar, Holidays, Export)
• Real-time calculations
• Beautiful visual displays

FEATURES:
• Gregorian, Islamic, Chinese, Hebrew calendars
• Accurate astronomical calculations
• Cultural holiday database
• CSV and iCal export
• Timezone support

The installer will place files in:
• /usr/local/bin/calendar (CLI)
• /Applications/calendar_gui.app (GUI)
• /usr/local/share/calendar/ (data)

Built: 2025-06-17 03:09:24 UTC
User: linhduongtuan
EOF

# Create conclusion message  
cat > pkg_resources/Conclusion.txt << 'EOF'
Multi-Calendar System v2.0.0 has been installed successfully!

QUICK START:
1. Open Terminal and type: calendar
2. Launch GUI from Applications folder
3. Check Desktop for shortcut

USAGE:
• CLI: Type 'calendar' in Terminal
• GUI: Open from Applications or Launchpad

FEATURES INSTALLED:
✅ Multiple calendar systems
✅ Lunar phase calculations
✅ Holiday database
✅ Export functionality
✅ Native macOS GUI

SUPPORT:
• Documentation: /usr/local/share/calendar/
• Bug reports: GitHub issues
• Community: Project discussions

Thank you for using Multi-Calendar System!
EOF

echo ""
echo "🔨 Building package components..."

# Build the main package
pkgbuild --root pkg_build \
         --scripts pkg_scripts \
         --identifier "$IDENTIFIER" \
         --version "$VERSION" \
         --install-location / \
         --ownership recommended \
         "MultiCalendarSystem.pkg"

if [[ $? -ne 0 ]]; then
    echo "❌ Failed to build package component"
    exit 1
fi

echo "✅ Package component built"

# Create distribution definition
cat > distribution.xml << EOF
<?xml version="1.0" encoding="utf-8"?>
<installer-script minSpecVersion="1.000000">
    <title>Multi-Calendar System v$VERSION</title>
    <organization>com.calendar</organization>
    <domains enable_anywhere="false" enable_currentUserHome="false" enable_localSystem="true"/>
    <options customize="never" allow-external-scripts="no"/>
    
    <welcome file="Welcome.txt" mime-type="text/plain"/>
    <conclusion file="Conclusion.txt" mime-type="text/plain"/>
    
    <pkg-ref id="$IDENTIFIER"/>
    
    <choices-outline>
        <line choice="choice1"/>
    </choices-outline>
    
    <choice id="choice1" title="Multi-Calendar System">
        <pkg-ref id="$IDENTIFIER"/>
    </choice>
    
    <pkg-ref id="$IDENTIFIER" installKBytes="$(du -k MultiCalendarSystem.pkg | cut -f1)">MultiCalendarSystem.pkg</pkg-ref>
</installer-script>
EOF

echo "🔨 Creating final installer package..."

# Build the final installer
productbuild --distribution distribution.xml \
            --resources pkg_resources \
            --package-path . \
            "$PKG_NAME"

if [[ $? -ne 0 ]]; then
    echo "❌ Failed to create final installer"
    exit 1
fi

# Clean up temporary files
rm -rf pkg_build pkg_scripts pkg_resources MultiCalendarSystem.pkg distribution.xml

if [[ -f "$PKG_NAME" ]]; then
    echo ""
    echo "✅ Successfully created: $PKG_NAME"
    echo "📊 Package size: $(ls -lh "$PKG_NAME" | awk '{print $5}')"
    echo ""
    echo "🎉 macOS installer package is ready!"
    echo ""
    echo "📋 Users can:"
    echo "   1. Download and double-click the .pkg file"
    echo "   2. Follow the installation wizard"
    echo "   3. Use 'calendar' command in Terminal"
    echo "   4. Launch GUI from Applications"
    echo ""
    echo "🔧 Test the package:"
    echo "   sudo installer -pkg '$PKG_NAME' -target /"
    echo "   (Note: This will install on your system)"
else
    echo "❌ Failed to create package"
    exit 1
fi