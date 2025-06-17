# install.sh - Universal installer script
#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INSTALL_PREFIX="/usr/local"
APP_DIR="/Applications"

echo "======================================"
echo "Multi-Calendar System v2.0 Installer"
echo "======================================"

# Check if running on macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos"
    echo "Detected: macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux"
    echo "Detected: Linux"
else
    echo "Unsupported platform: $OSTYPE"
    exit 1
fi

# Function to install command-line tools
install_cli() {
    echo ""
    echo "Installing command-line tools..."
    
    # Create directories
    sudo mkdir -p "$INSTALL_PREFIX/bin"
    sudo mkdir -p "$INSTALL_PREFIX/lib"
    sudo mkdir -p "$INSTALL_PREFIX/share/calendar"
    sudo mkdir -p "$INSTALL_PREFIX/share/man/man1"
    
    # Install binaries
    sudo cp "$SCRIPT_DIR/build/calendar" "$INSTALL_PREFIX/bin/"
    sudo cp "$SCRIPT_DIR/build/libcalendar_lib.a" "$INSTALL_PREFIX/lib/"
    
    # Install data files
    sudo cp -r "$SCRIPT_DIR/data/" "$INSTALL_PREFIX/share/calendar/"
    
    # Install manual page
    sudo cp "$SCRIPT_DIR/docs/calendar.1" "$INSTALL_PREFIX/share/man/man1/" 2>/dev/null || true
    
    # Set permissions
    sudo chmod 755 "$INSTALL_PREFIX/bin/calendar"
    sudo chmod 644 "$INSTALL_PREFIX/lib/libcalendar_lib.a"
    
    echo "✅ Command-line tools installed to $INSTALL_PREFIX/bin/calendar"
}

# Function to install macOS GUI app
install_macos_gui() {
    if [[ "$PLATFORM" == "macos" ]]; then
        echo ""
        echo "Installing macOS GUI application..."
        
        # Copy app bundle
        sudo cp -r "$SCRIPT_DIR/build/calendar_gui.app" "$APP_DIR/"
        
        # Set permissions
        sudo chmod -R 755 "$APP_DIR/calendar_gui.app"
        sudo chown -R root:wheel "$APP_DIR/calendar_gui.app"
        
        echo "✅ GUI application installed to $APP_DIR/calendar_gui.app"
        echo "📱 You can now launch it from Applications folder"
    fi
}

# Function to create desktop shortcut
create_desktop_shortcut() {
    if [[ "$PLATFORM" == "macos" ]]; then
        USER_DESKTOP="$HOME/Desktop"
        if [[ -d "$USER_DESKTOP" ]]; then
            cat > "$USER_DESKTOP/Multi-Calendar System.command" << 'EOF'
#!/bin/bash
/usr/local/bin/calendar
EOF
            chmod +x "$USER_DESKTOP/Multi-Calendar System.command"
            echo "🖥️  Desktop shortcut created"
        fi
    fi
}

# Main installation process
echo ""
echo "Choose installation type:"
echo "1) Full installation (CLI + GUI)"
echo "2) Command-line only"
echo "3) GUI only (macOS)"
echo ""
read -p "Enter choice [1-3]: " choice

case $choice in
    1)
        install_cli
        install_macos_gui
        create_desktop_shortcut
        ;;
    2)
        install_cli
        create_desktop_shortcut
        ;;
    3)
        if [[ "$PLATFORM" == "macos" ]]; then
            install_macos_gui
        else
            echo "❌ GUI installation only available on macOS"
            exit 1
        fi
        ;;
    *)
        echo "❌ Invalid choice"
        exit 1
        ;;
esac

echo ""
echo "🎉 Installation completed successfully!"
echo ""
echo "📋 Usage:"
echo "   Command-line: calendar"
echo "   GUI (macOS):  Open 'Multi-Calendar System' from Applications"
echo ""
echo "📚 For help: calendar --help"
echo "🔧 To uninstall: sudo $SCRIPT_DIR/uninstall.sh"