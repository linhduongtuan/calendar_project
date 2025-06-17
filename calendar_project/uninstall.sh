# uninstall.sh
#!/bin/bash

echo "======================================"
echo "Multi-Calendar System Uninstaller"
echo "======================================"

# Remove command-line tools
echo "Removing command-line tools..."
sudo rm -f /usr/local/bin/calendar
sudo rm -f /usr/local/lib/libcalendar_lib.a
sudo rm -rf /usr/local/share/calendar/
sudo rm -f /usr/local/share/man/man1/calendar.1

# Remove macOS GUI app
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Removing GUI application..."
    sudo rm -rf /Applications/calendar_gui.app
fi

# Remove desktop shortcuts
rm -f "$HOME/Desktop/Multi-Calendar System.command"

echo "✅ Multi-Calendar System has been uninstalled"