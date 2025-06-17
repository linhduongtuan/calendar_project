// src/gui/gtk_calendar.h
#ifndef GTK_CALENDAR_H
#define GTK_CALENDAR_H

#include <gtk/gtk.h>
#include "../../include/calendar_types.h"

// GUI structures
typedef struct {
    GtkWidget *window;
    GtkWidget *notebook;
    GtkWidget *calendar_widget;
    GtkWidget *date_label;
    GtkWidget *conversion_frame;
    CalendarType current_calendar;
} CalendarGUI;

// GUI functions
CalendarGUI* calendar_gui_create(void);
void calendar_gui_destroy(CalendarGUI* gui);
void calendar_gui_run(CalendarGUI* gui);

// Event handlers
void on_calendar_changed(GtkCalendar *calendar, CalendarGUI *gui);
void on_convert_button_clicked(GtkButton *button, CalendarGUI *gui);
void on_export_button_clicked(GtkButton *button, CalendarGUI *gui);

#endif // GTK_CALENDAR_H