// src/gui/gtk_calendar.c
#include "gtk_calendar.h"
#include "../calendars/gregorian.h"
#include "../calendars/islamic.h"
#include "../calendars/chinese.h"
#include "../calendars/hebrew.h"

CalendarGUI* calendar_gui_create(void) {
    CalendarGUI* gui = malloc(sizeof(CalendarGUI));
    if (!gui) return NULL;
    
    // Create main window
    gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gui->window), "Multi-Calendar System");
    gtk_window_set_default_size(GTK_WINDOW(gui->window), 800, 600);
    
    // Create notebook for different calendar views
    gui->notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(gui->window), gui->notebook);
    
    // Create Gregorian calendar page
    GtkWidget *gregorian_page = create_gregorian_page(gui);
    gtk_notebook_append_page(GTK_NOTEBOOK(gui->notebook), 
                            gregorian_page, gtk_label_new("Gregorian"));
    
    // Create Islamic calendar page
    GtkWidget *islamic_page = create_islamic_page(gui);
    gtk_notebook_append_page(GTK_NOTEBOOK(gui->notebook), 
                            islamic_page, gtk_label_new("Islamic"));
    
    // Create Chinese calendar page
    GtkWidget *chinese_page = create_chinese_page(gui);
    gtk_notebook_append_page(GTK_NOTEBOOK(gui->notebook), 
                            chinese_page, gtk_label_new("Chinese"));
    
    // Create Hebrew calendar page
    GtkWidget *hebrew_page = create_hebrew_page(gui);
    gtk_notebook_append_page(GTK_NOTEBOOK(gui->notebook), 
                            hebrew_page, gtk_label_new("Hebrew"));
    
    // Create conversion page
    GtkWidget *conversion_page = create_conversion_page(gui);
    gtk_notebook_append_page(GTK_NOTEBOOK(gui->notebook), 
                            conversion_page, gtk_label_new("Conversions"));
    
    // Connect signals
    g_signal_connect(gui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gui->current_calendar = CALENDAR_GREGORIAN;
    
    return gui;
}

GtkWidget* create_gregorian_page(CalendarGUI* gui) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
    // Calendar widget
    gui->calendar_widget = gtk_calendar_new();
    gtk_box_pack_start(GTK_BOX(vbox), gui->calendar_widget, FALSE, FALSE, 0);
    
    // Date display label
    gui->date_label = gtk_label_new("Select a date");
    gtk_box_pack_start(GTK_BOX(vbox), gui->date_label, FALSE, FALSE, 0);
    
    // Connect calendar signal
    g_signal_connect(gui->calendar_widget, "day-selected", 
                     G_CALLBACK(on_calendar_changed), gui);
    
    return vbox;
}

void on_calendar_changed(GtkCalendar *calendar, CalendarGUI *gui) {
    guint year, month, day;
    gtk_calendar_get_date(calendar, &year, &month, &day);
    month++; // GTK months are 0-based
    
    // Update date label with current calendar format
    char date_text[200];
    switch (gui->current_calendar) {
        case CALENDAR_GREGORIAN: {
            GregorianDate* greg_date = gregorian_create_date(day, month, year);
            if (greg_date) {
                snprintf(date_text, sizeof(date_text), 
                        "Gregorian: %s, %s %d, %d",
                        gregorian_days[greg_date->day_of_week],
                        gregorian_months[month - 1], day, year);
                gregorian_destroy_date(greg_date);
            }
            break;
        }
        // Add other calendar cases...
    }
    
    gtk_label_set_text(GTK_LABEL(gui->date_label), date_text);
}

void calendar_gui_run(CalendarGUI* gui) {
    if (!gui) return;
    
    gtk_widget_show_all(gui->window);
    gtk_main();
}

// Main GUI application
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    CalendarGUI* gui = calendar_gui_create();
    if (gui) {
        calendar_gui_run(gui);
        calendar_gui_destroy(gui);
    }
    
    return 0;
}