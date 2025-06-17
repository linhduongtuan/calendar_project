# CMakeLists.txt (Fixed console UI linking)
cmake_minimum_required(VERSION 3.10)
project(CalendarSystemAdvanced VERSION 2.0.0 LANGUAGES C)

# Options for optional features
option(BUILD_GUI "Build GUI interface" OFF)
option(BUILD_ASTRONOMY "Build astronomical calculations" ON)
option(BUILD_HOLIDAYS "Build holiday database support" ON)
option(BUILD_TIMEZONE "Build timezone support" ON)
option(BUILD_EXPORT "Build export functionality" ON)

# Detect platform and set GUI framework
if(BUILD_GUI)
    if(APPLE)
        # Use Cocoa on macOS
        enable_language(OBJC)
        find_library(COCOA_LIBRARY Cocoa)
        find_library(FOUNDATION_LIBRARY Foundation)
        if(COCOA_LIBRARY AND FOUNDATION_LIBRARY)
            message(STATUS "Cocoa found - macOS native GUI will be built")
            set(GUI_FRAMEWORK "COCOA")
        else()
            message(WARNING "Cocoa not found - GUI will be disabled")
            set(BUILD_GUI OFF)
        endif()
    elseif(WIN32)
        # Use Win32 API on Windows
        message(STATUS "Windows detected - Win32 GUI support not implemented yet")
        set(BUILD_GUI OFF)
    else()
        # Use GTK on Linux
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
        if(GTK3_FOUND)
            message(STATUS "GTK3 found - GTK GUI will be built")
            set(GUI_FRAMEWORK "GTK")
        else()
            message(WARNING "GTK3 not found - GUI will be disabled")
            set(BUILD_GUI OFF)
        endif()
    endif()
endif()

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE CALENDAR_SOURCES "src/calendars/*.c")
file(GLOB_RECURSE UTILS_SOURCES "src/utils/*.c")
file(GLOB_RECURSE UI_SOURCES "src/ui/*.c")  # Always include console UI

# Optional source files
if(BUILD_ASTRONOMY)
    file(GLOB_RECURSE ASTRONOMY_SOURCES "src/astronomy/*.c")
    list(APPEND UTILS_SOURCES ${ASTRONOMY_SOURCES})
    add_definitions(-DBUILD_ASTRONOMY)
endif()

if(BUILD_HOLIDAYS)
    file(GLOB_RECURSE HOLIDAY_SOURCES "src/holidays/*.c")
    list(APPEND UTILS_SOURCES ${HOLIDAY_SOURCES})
    add_definitions(-DBUILD_HOLIDAYS)
endif()

if(BUILD_TIMEZONE)
    file(GLOB_RECURSE TIMEZONE_SOURCES "src/timezone/*.c")
    list(APPEND UTILS_SOURCES ${TIMEZONE_SOURCES})
    add_definitions(-DBUILD_TIMEZONE)
endif()

if(BUILD_EXPORT)
    file(GLOB_RECURSE EXPORT_SOURCES "src/export/*.c")
    list(APPEND UTILS_SOURCES ${EXPORT_SOURCES})
    add_definitions(-DBUILD_EXPORT)
endif()

# GUI sources based on platform
if(BUILD_GUI)
    if(GUI_FRAMEWORK STREQUAL "COCOA")
        file(GLOB_RECURSE GUI_SOURCES "src/gui/cocoa_*.m")
        add_definitions(-DBUILD_GUI -DGUI_COCOA)
    elseif(GUI_FRAMEWORK STREQUAL "GTK")
        file(GLOB_RECURSE GUI_SOURCES "src/gui/gtk_*.c")
        include_directories(${GTK3_INCLUDE_DIRS})
        add_definitions(-DBUILD_GUI -DGUI_GTK)
    endif()
endif()

# Create library with all sources including UI
set(LIB_SOURCES ${CALENDAR_SOURCES} ${UTILS_SOURCES} ${UI_SOURCES})
add_library(calendar_lib STATIC ${LIB_SOURCES})
target_link_libraries(calendar_lib m)

# Console application (always include console UI)
add_executable(calendar src/main.c)
target_link_libraries(calendar calendar_lib)

# GUI application
if(BUILD_GUI)
    if(GUI_FRAMEWORK STREQUAL "COCOA")
        add_executable(calendar_gui src/gui_main.m ${GUI_SOURCES})
        target_link_libraries(calendar_gui calendar_lib ${COCOA_LIBRARY} ${FOUNDATION_LIBRARY})
        # Simplified without Info.plist for now
        set_target_properties(calendar_gui PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "Multi-Calendar System"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.calendar.multicalendar"
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        )
    elseif(GUI_FRAMEWORK STREQUAL "GTK")
        add_executable(calendar_gui src/gui_main.c ${GUI_SOURCES})
        target_link_libraries(calendar_gui calendar_lib ${GTK3_LIBRARIES})
        target_compile_options(calendar_gui PRIVATE ${GTK3_CFLAGS_OTHER})
    endif()
endif()

# Feature summary
message(STATUS "Calendar System Advanced Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  GUI Interface: ${BUILD_GUI}")
if(BUILD_GUI)
    message(STATUS "  GUI Framework: ${GUI_FRAMEWORK}")
endif()
message(STATUS "  Astronomical Calculations: ${BUILD_ASTRONOMY}")
message(STATUS "  Holiday Database: ${BUILD_HOLIDAYS}")
message(STATUS "  Timezone Support: ${BUILD_TIMEZONE}")
message(STATUS "  Export Functionality: ${BUILD_EXPORT}")