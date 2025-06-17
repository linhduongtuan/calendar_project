// src/ui/console_ui.h (Updated with proper includes)
#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "../../include/calendar_types.h"
#include "../calendars/gregorian.h"
#include "../calendars/islamic.h"
#include "../calendars/chinese.h"
#include "../calendars/hebrew.h"
#include "../utils/date_utils.h"  // Add this include

// Main UI functions
void console_ui_run(void);
void console_ui_show_menu(void);
int console_ui_get_choice(void);

// Menu handlers
void handle_gregorian_menu(void);
void handle_islamic_menu(void);
void handle_chinese_menu(void);
void handle_hebrew_menu(void);
void handle_conversion_menu(void);
void handle_date_calculator(void);

// Utility functions
void clear_input_buffer(void);
int get_date_input(int* day, int* month, int* year);

#endif // CONSOLE_UI_H