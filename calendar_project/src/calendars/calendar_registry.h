#ifndef CALENDAR_REGISTRY_H
#define CALENDAR_REGISTRY_H

#include "../../include/calendar_types_extended.h"

// Calendar registry for dynamic calendar system management
typedef struct {
    CalendarSystemEntry* systems;
    int count;
    int capacity;
} CalendarRegistry;

// Registry management functions
CalendarRegistry* calendar_registry_create(void);
void calendar_registry_destroy(CalendarRegistry* registry);

// Calendar system registration
CalendarResult calendar_registry_register_system(CalendarRegistry* registry, 
                                                const CalendarSystemEntry* system);
CalendarResult calendar_registry_unregister_system(CalendarRegistry* registry, 
                                                  ExtendedCalendarType type);

// Calendar system lookup
CalendarSystemEntry* calendar_registry_get_system(CalendarRegistry* registry, 
                                                 ExtendedCalendarType type);
int calendar_registry_is_supported(CalendarRegistry* registry, ExtendedCalendarType type);

// Calendar operations through registry
void* calendar_registry_create_date(CalendarRegistry* registry, ExtendedCalendarType type,
                                   int day, int month, int year);
void calendar_registry_destroy_date(CalendarRegistry* registry, ExtendedCalendarType type, 
                                   void* date);
CalendarResult calendar_registry_validate_date(CalendarRegistry* registry, 
                                              ExtendedCalendarType type,
                                              int day, int month, int year);
void calendar_registry_print_date(CalendarRegistry* registry, ExtendedCalendarType type, 
                                 const void* date);

// Conversion operations
CalendarResult calendar_registry_convert_date(CalendarRegistry* registry,
                                             ExtendedCalendarType from_type, const void* from_date,
                                             ExtendedCalendarType to_type, void* to_date);

// Multi-calendar operations
MultiCalendarDate* calendar_registry_create_multi_date(CalendarRegistry* registry, 
                                                      long julian_day);
void calendar_registry_destroy_multi_date(MultiCalendarDate* multi_date);

// Registry information
void calendar_registry_list_systems(CalendarRegistry* registry);
int calendar_registry_get_system_count(CalendarRegistry* registry);
CalendarCapabilities* calendar_registry_get_capabilities(CalendarRegistry* registry, 
                                                        ExtendedCalendarType type);

// Default registry initialization
CalendarRegistry* calendar_registry_create_default(void);
CalendarResult calendar_registry_register_all_standard_systems(CalendarRegistry* registry);

// Individual system registration functions
CalendarResult calendar_registry_register_gregorian(CalendarRegistry* registry);
CalendarResult calendar_registry_register_islamic(CalendarRegistry* registry);
CalendarResult calendar_registry_register_chinese(CalendarRegistry* registry);
CalendarResult calendar_registry_register_hebrew(CalendarRegistry* registry);
CalendarResult calendar_registry_register_persian(CalendarRegistry* registry);
CalendarResult calendar_registry_register_buddhist(CalendarRegistry* registry);
CalendarResult calendar_registry_register_ethiopian(CalendarRegistry* registry);
CalendarResult calendar_registry_register_maya(CalendarRegistry* registry);

// Utility functions
const char* calendar_registry_get_system_name(ExtendedCalendarType type);
const char* calendar_registry_get_system_description(ExtendedCalendarType type);
ExtendedCalendarType calendar_registry_parse_system_name(const char* name);

// Configuration and preferences
typedef struct {
    ExtendedCalendarType default_calendar;
    ExtendedCalendarType* display_calendars;
    int display_calendar_count;
    int show_astronomical_events;
    int show_holidays;
    int use_local_timezone;
    double default_latitude;
    double default_longitude;
    char default_country[4];
} CalendarPreferences;

CalendarPreferences* calendar_preferences_create_default(void);
void calendar_preferences_destroy(CalendarPreferences* prefs);
CalendarResult calendar_preferences_load_from_file(CalendarPreferences* prefs, const char* filename);
CalendarResult calendar_preferences_save_to_file(const CalendarPreferences* prefs, const char* filename);

#endif // CALENDAR_REGISTRY_H