# create_dmg.sh - Simplified DMG creator
#!/bin/bash

VERSION="2.0.0"
DMG_NAME="multi-calendar-system-$VERSION.dmg"

echo "======================================"
echo "Creating macOS DMG Package"
echo "======================================"

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ DMG creation only works on macOS"
    exit 1
fi

# Check if GUI app exists
if [[ ! -d "build/calendar_gui.app" ]]; then
    echo "❌ GUI app not found at build/calendar_gui.app"
    echo "Please ensure the GUI was built successfully"
    exit 1
fi

# Check if hdiutil exists
if ! command -v hdiutil &> /dev/null; then
    echo "❌ hdiutil command not found"
    exit 1
fi

echo "✅ macOS detected"
echo "✅ GUI app found"
echo "✅ hdiutil available"

# Create temporary DMG directory
DMG_DIR="dmg_temp"
echo ""
echo "Creating DMG structure..."

# Clean up any existing temp directory
rm -rf "$DMG_DIR"
mkdir -p "$DMG_DIR"

# Copy the GUI app
echo "Copying GUI application..."
cp -r build/calendar_gui.app "$DMG_DIR/Multi-Calendar System.app"

# Create a nice background and layout
mkdir -p "$DMG_DIR/.background"

# Create Applications symlink
echo "Creating Applications symlink..."
ln -sf /Applications "$DMG_DIR/Applications"

# Create installer for CLI tools
echo "Creating CLI installer..."
cat > "$DMG_DIR/Install CLI Tools.command" << 'EOF'
#!/bin/bash

echo "======================================"
echo "Multi-Calendar System CLI Installer"
echo "======================================"
echo ""
echo "This will install the command-line version of Multi-Calendar System."
echo "You'll need administrator privileges."
echo ""
read -p "Continue? (y/N): " confirm

if [[ "$confirm" != "y" && "$confirm" != "Y" ]]; then
    echo "Installation cancelled."
    exit 0
fi

# Check if we can find the full package
echo ""
echo "Please download the full package (.tar.gz) to install CLI tools."
echo "Available at: https://github.com/your-repo/multi-calendar-system/releases"
echo ""
echo "Or build from source:"
echo "  git clone <repository>"
echo "  cd calendar_project/build"
echo "  cmake -DBUILD_GUI=ON -DBUILD_ASTRONOMY=ON .."
echo "  make"
echo "  sudo make install"
echo ""

read -p "Press Enter to close..."
EOF

chmod +x "$DMG_DIR/Install CLI Tools.command"

# Create README
cat > "$DMG_DIR/README.txt" << 'EOF'
Multi-Calendar System v2.0.0
=============================

QUICK INSTALL:
1. Drag "Multi-Calendar System.app" to Applications folder
2. Launch from Applications or Launchpad

FEATURES:
• Multiple calendar systems (Gregorian, Islamic, Chinese, Hebrew)
• Accurate calendar conversions
• Lunar phase calculations
• Holiday database
• Native macOS interface with 3 tabs:
  - Calendar: Live date conversions
  - Holidays: Cultural celebrations
  - Export: Save data as CSV

COMMAND-LINE VERSION:
• Run "Install CLI Tools.command" for terminal access
• Or download full package from GitHub

USAGE:
• GUI: Launch from Applications
• CLI: Type "calendar" in Terminal (after CLI installation)

Built: 2025-06-17 02:42:45 UTC
User: linhduongtuan
EOF

echo "Setting up DMG contents..."

# Remove any existing DMG
rm -f "$DMG_NAME"

# Create the DMG
echo ""
echo "Creating DMG image..."
echo "This may take a moment..."

# First create a read-write DMG
hdiutil create -volname "Multi-Calendar System $VERSION" \
               -srcfolder "$DMG_DIR" \
               -ov -format UDRW \
               -size 100m \
               "temp_$DMG_NAME"

if [[ $? -ne 0 ]]; then
    echo "❌ Failed to create temporary DMG"
    rm -rf "$DMG_DIR"
    exit 1
fi

# Mount the DMG
echo "Mounting DMG for customization..."
MOUNT_DIR="/Volumes/Multi-Calendar System $VERSION"
hdiutil attach "temp_$DMG_NAME"

# Wait for mount
sleep 2

# Set up the DMG appearance (optional)
if [[ -d "$MOUNT_DIR" ]]; then
    echo "Customizing DMG appearance..."
    
    # Set icon positions using AppleScript (optional)
    osascript << 'APPLESCRIPT'
tell application "Finder"
    try
        set dmg_window to window "Multi-Calendar System 2.0.0"
        set current view of dmg_window to icon view
        set toolbar visible of dmg_window to false
        set statusbar visible of dmg_window to false
        set the bounds of dmg_window to {100, 100, 600, 400}
        
        set icon size of the icon view options of dmg_window to 128
        set arrangement of the icon view options of dmg_window to not arranged
        
        -- Position icons
        set position of item "Multi-Calendar System.app" of dmg_window to {150, 150}
        set position of item "Applications" of dmg_window to {350, 150}
        set position of item "Install CLI Tools.command" of dmg_window to {150, 250}
        set position of item "README.txt" of dmg_window to {350, 250}
        
        update dmg_window without registering applications
        delay 2
    on error
        -- Ignore AppleScript errors
    end try
end tell
APPLESCRIPT

    sleep 1
fi

# Unmount the DMG
echo "Unmounting DMG..."
hdiutil detach "$MOUNT_DIR" -quiet

# Convert to compressed read-only DMG
echo "Compressing DMG..."
hdiutil convert "temp_$DMG_NAME" -format UDZO -o "$DMG_NAME"

# Clean up
rm -f "temp_$DMG_NAME"
rm -rf "$DMG_DIR"

if [[ -f "$DMG_NAME" ]]; then
    echo ""
    echo "✅ Successfully created: $DMG_NAME"
    echo "📊 DMG size: $(ls -lh "$DMG_NAME" | awk '{print $5}')"
    echo ""
    echo "🎉 DMG is ready for distribution!"
    echo ""
    echo "📋 Users can:"
    echo "   1. Download and mount the DMG"
    echo "   2. Drag the app to Applications"
    echo "   3. Launch from Applications folder"
    echo ""
    echo "🔧 Test the DMG:"
    echo "   open '$DMG_NAME'"
else
    echo "❌ Failed to create DMG"
    exit 1
fi