// src/gui/cocoa_calendar.m (Fixed with proper includes and UTType fix)
#import "cocoa_calendar.h"
#include "../holidays/holiday_db.h"
#include "../calendars/gregorian.h"
#include "../calendars/islamic.h"
#include "../calendars/chinese.h"
#include "../calendars/hebrew.h"
#include "../astronomy/lunar_calc.h"
#include "../export/calendar_export.h"
#include "../utils/date_utils.h"  // Add this include for julian_day_to_date

// Import UTType for macOS 12+
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

@implementation CalendarController {
    HolidayDatabase* holidayDB;
}

// NSApplicationDelegate methods
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSLog(@"Multi-Calendar System GUI started successfully");
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    if (holidayDB) {
        holiday_db_destroy(holidayDB);
        holidayDB = NULL;
    }
    NSLog(@"Multi-Calendar System GUI terminated");
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Initialize holiday database
    [self loadHolidayDatabase];
    
    [self setupUI];
    [self updateConversions];
}

- (void)loadHolidayDatabase {
    // Create data directory if it doesn't exist
    NSString* dataDir = @"../data";
    NSFileManager* fileManager = [NSFileManager defaultManager];
    
    if (![fileManager fileExistsAtPath:dataDir]) {
        [fileManager createDirectoryAtPath:dataDir 
                withIntermediateDirectories:YES 
                attributes:nil 
                error:nil];
    }
    
    // Create sample holiday file
    NSString* holidayFile = [dataDir stringByAppendingPathComponent:@"holidays.csv"];
    
    if (![fileManager fileExistsAtPath:holidayFile]) {
        NSString* sampleData = @"Name,Description,CalendarType,HolidayType,Month,Day,Country,IsPublic\n"
                              @"New Year's Day,Beginning of the year,0,0,1,1,US,1\n"
                              @"Independence Day,US Independence Day,0,0,7,4,US,1\n"
                              @"Christmas Day,Christian celebration,0,0,12,25,US,1\n"
                              @"Chinese New Year,Lunar New Year,2,0,1,1,CN,1\n"
                              @"Eid al-Fitr,End of Ramadan,1,0,10,1,SA,1\n"
                              @"Rosh Hashanah,Jewish New Year,3,0,1,1,IL,1\n";
        
        [sampleData writeToFile:holidayFile 
                     atomically:YES 
                       encoding:NSUTF8StringEncoding 
                          error:nil];
    }
    
    // Load using C function
    holidayDB = holiday_db_create();
    if (holidayDB) {
        holiday_db_load_from_file(holidayDB, [holidayFile UTF8String]);
        NSLog(@"Holiday database loaded successfully with %d holidays", holidayDB->count);
    } else {
        NSLog(@"Failed to create holiday database");
    }
}

- (void)setupUI {
    self.view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)];
    
    // Create tab view
    NSTabView *tabView = [[NSTabView alloc] initWithFrame:NSMakeRect(10, 10, 780, 580)];
    
    // Tab 1: Main Calendar
    NSTabViewItem *mainTab = [[NSTabViewItem alloc] initWithIdentifier:@"main"];
    [mainTab setLabel:@"Calendar"];
    NSView *mainView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 760, 550)];
    
    // Add title label
    NSTextField *titleLabel = [[NSTextField alloc] initWithFrame:NSMakeRect(20, 510, 720, 25)];
    [titleLabel setStringValue:@"Multi-Calendar System v2.0 - Select a date to see conversions"];
    [titleLabel setEditable:NO];
    [titleLabel setBordered:NO];
    [titleLabel setBackgroundColor:[NSColor controlColor]];
    [titleLabel setFont:[NSFont boldSystemFontOfSize:14]];
    [mainView addSubview:titleLabel];
    
    // Date picker
    self.datePicker = [[NSDatePicker alloc] initWithFrame:NSMakeRect(20, 480, 200, 25)];
    [self.datePicker setDatePickerStyle:NSDatePickerStyleTextFieldAndStepper];
    [self.datePicker setTarget:self];
    [self.datePicker setAction:@selector(dateChanged:)];
    [mainView addSubview:self.datePicker];
    
    // Conversion text view
    NSScrollView *conversionScrollView = [[NSScrollView alloc] initWithFrame:NSMakeRect(20, 200, 720, 270)];
    [conversionScrollView setHasVerticalScroller:YES];
    [conversionScrollView setBorderType:NSBezelBorder];
    
    self.conversionTextView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 0, 700, 250)];
    [self.conversionTextView setEditable:NO];
    [self.conversionTextView setFont:[NSFont fontWithName:@"Monaco" size:12]];
    [conversionScrollView setDocumentView:self.conversionTextView];
    [mainView addSubview:conversionScrollView];
    
    // Moon phase view
    NSScrollView *moonScrollView = [[NSScrollView alloc] initWithFrame:NSMakeRect(20, 20, 720, 170)];
    [moonScrollView setHasVerticalScroller:YES];
    [moonScrollView setBorderType:NSBezelBorder];
    
    self.moonPhaseTextView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 0, 700, 150)];
    [self.moonPhaseTextView setEditable:NO];
    [self.moonPhaseTextView setFont:[NSFont fontWithName:@"Monaco" size:11]];
    [moonScrollView setDocumentView:self.moonPhaseTextView];
    [mainView addSubview:moonScrollView];
    
    [mainTab setView:mainView];
    [tabView addTabViewItem:mainTab];
    
    // Tab 2: Holidays
    NSTabViewItem *holidayTab = [[NSTabViewItem alloc] initWithIdentifier:@"holidays"];
    [holidayTab setLabel:@"Holidays"];
    NSView *holidayView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 760, 550)];
    
    NSScrollView *holidayScrollView = [[NSScrollView alloc] initWithFrame:NSMakeRect(20, 20, 720, 510)];
    [holidayScrollView setHasVerticalScroller:YES];
    [holidayScrollView setBorderType:NSBezelBorder];
    
    self.holidayTextView = [[NSTextView alloc] initWithFrame:NSMakeRect(0, 0, 700, 490)];
    [self.holidayTextView setEditable:NO];
    [self.holidayTextView setFont:[NSFont fontWithName:@"Monaco" size:11]];
    [holidayScrollView setDocumentView:self.holidayTextView];
    [holidayView addSubview:holidayScrollView];
    
    [holidayTab setView:holidayView];
    [tabView addTabViewItem:holidayTab];
    
    // Tab 3: Export
    NSTabViewItem *exportTab = [[NSTabViewItem alloc] initWithIdentifier:@"export"];
    [exportTab setLabel:@"Export"];
    NSView *exportView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 760, 550)];
    
    // Export title
    NSTextField *exportTitle = [[NSTextField alloc] initWithFrame:NSMakeRect(20, 510, 720, 25)];
    [exportTitle setStringValue:@"Export Calendar Data"];
    [exportTitle setEditable:NO];
    [exportTitle setBordered:NO];
    [exportTitle setBackgroundColor:[NSColor controlColor]];
    [exportTitle setFont:[NSFont boldSystemFontOfSize:16]];
    [exportView addSubview:exportTitle];
    
    // Export buttons
    NSButton *exportCSVButton = [[NSButton alloc] initWithFrame:NSMakeRect(20, 470, 150, 32)];
    [exportCSVButton setTitle:@"Export to CSV"];
    [exportCSVButton setTarget:self];
    [exportCSVButton setAction:@selector(exportToCSV:)];
    [exportView addSubview:exportCSVButton];
    
    NSButton *exportICalButton = [[NSButton alloc] initWithFrame:NSMakeRect(180, 470, 150, 32)];
    [exportICalButton setTitle:@"Export to iCal"];
    [exportICalButton setTarget:self];
    [exportICalButton setAction:@selector(exportToICal:)];
    [exportView addSubview:exportICalButton];
    
    // Export status label
    self.exportStatusLabel = [[NSTextField alloc] initWithFrame:NSMakeRect(20, 430, 720, 25)];
    [self.exportStatusLabel setEditable:NO];
    [self.exportStatusLabel setBordered:NO];
    [self.exportStatusLabel setBackgroundColor:[NSColor controlColor]];
    [self.exportStatusLabel setStringValue:@"Ready to export..."];
    [exportView addSubview:self.exportStatusLabel];
    
    [exportTab setView:exportView];
    [tabView addTabViewItem:exportTab];
    
    [self.view addSubview:tabView];
    
    // Load initial holiday data
    [self updateHolidayDisplay];
}

- (void)dateChanged:(id)sender {
    [self updateConversions];
}

- (void)updateConversions {
    NSDate *selectedDate = [self.datePicker dateValue];
    NSCalendar *gregorianCalendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    NSDateComponents *components = [gregorianCalendar components:(NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear) 
                                                        fromDate:selectedDate];
    
    int day = (int)[components day];
    int month = (int)[components month];
    int year = (int)[components year];
    
    // Create C date structure
    GregorianDate* gregDate = gregorian_create_date(day, month, year);
    if (!gregDate) return;
    
    NSMutableString *conversions = [[NSMutableString alloc] init];
    
    // Gregorian info
    [conversions appendFormat:@"=== SELECTED DATE CONVERSIONS ===\n\n"];
    [conversions appendFormat:@"Gregorian: %s, %s %d, %d\n", 
     gregorian_days[gregDate->day_of_week], 
     gregorian_months[month-1], day, year];
    [conversions appendFormat:@"Julian Day: %ld\n\n", gregDate->julian_day];
    
    // Islamic conversion
    IslamicDate islamicDate;
    if (islamic_from_gregorian(gregDate, &islamicDate) == CALENDAR_SUCCESS) {
        [conversions appendFormat:@"Islamic: %d %s %d AH\n", 
         islamicDate.base.day, islamicDate.month_name, islamicDate.base.year];
    }
    
    // Chinese conversion
    ChineseDate chineseDate;
    if (chinese_from_gregorian(gregDate, &chineseDate) == CALENDAR_SUCCESS) {
        [conversions appendFormat:@"Chinese: Year of %s (%s), Cycle %d\n", 
         chineseDate.zodiac, chineseDate.stem_branch, chineseDate.cycle_year];
    }
    
    // Hebrew conversion
    HebrewDate hebrewDate;
    if (hebrew_from_gregorian(gregDate, &hebrewDate) == CALENDAR_SUCCESS) {
        [conversions appendFormat:@"Hebrew: %d %s %d %s\n", 
         hebrewDate.base.day, hebrewDate.month_name, hebrewDate.base.year,
         hebrewDate.is_leap_year ? "(Leap Year)" : ""];
    }
    
    [self.conversionTextView setString:conversions];
    
    // Update moon phase
    [self updateMoonPhase:gregDate->julian_day];
    
    gregorian_destroy_date(gregDate);
}

- (void)updateMoonPhase:(long)julianDay {
    NSMutableString *moonInfo = [[NSMutableString alloc] init];
    
    [moonInfo appendString:@"=== LUNAR INFORMATION ===\n\n"];
    
    // Basic moon phase
    double moonAge = lunar_calculate_moon_age(julianDay);
    LunarPhase phase = lunar_get_phase(moonAge);
    double illumination = lunar_calculate_illumination(moonAge);
    
    [moonInfo appendFormat:@"Moon Phase: %s\n", lunar_phase_name(phase)];
    [moonInfo appendFormat:@"Moon Age: %.2f days\n", moonAge];
    [moonInfo appendFormat:@"Illumination: %.1f%%\n", illumination * 100];
    
    // Precise calculations
    PreciseLunarInfo* precise = lunar_calculate_precise(julianDay);
    if (precise) {
        [moonInfo appendFormat:@"Phase Angle: %.1f degrees\n", precise->moon_phase_angle];
        [moonInfo appendFormat:@"Distance: %.0f km\n", precise->moon_distance_km];
        [moonInfo appendFormat:@"Precise Age: %.3f days\n", precise->moon_age_precise];
        
        free(precise);
    }
    
    // Next lunar events
    double nextNew = lunar_next_new_moon(julianDay);
    double nextFull = lunar_next_full_moon(julianDay);
    
    int newDay, newMonth, newYear;
    int fullDay, fullMonth, fullYear;
    
    julian_day_to_date((long)nextNew, &newDay, &newMonth, &newYear);
    julian_day_to_date((long)nextFull, &fullDay, &fullMonth, &fullYear);
    
    [moonInfo appendFormat:@"\nNext New Moon: %d/%d/%d\n", newDay, newMonth, newYear];
    [moonInfo appendFormat:@"Next Full Moon: %d/%d/%d\n", fullDay, fullMonth, fullYear];
    
    [self.moonPhaseTextView setString:moonInfo];
}

- (void)updateHolidayDisplay {
    NSMutableString *holidayInfo = [[NSMutableString alloc] init];
    
    [holidayInfo appendString:@"=== HOLIDAY DATABASE ===\n\n"];
    
    if (holidayDB && holidayDB->count > 0) {
        [holidayInfo appendFormat:@"Total holidays loaded: %d\n\n", holidayDB->count];
        
        for (int i = 0; i < holidayDB->count; i++) {
            Holiday* holiday = &holidayDB->holidays[i];
            
            [holidayInfo appendFormat:@"%s (%s)\n", holiday->name, holiday->country_code];
            [holidayInfo appendFormat:@"  Date: %d/%d\n", holiday->date.day, holiday->date.month];
            [holidayInfo appendFormat:@"  Type: %s\n", holiday->is_public_holiday ? "Public" : "Cultural"];
            
            const char* calendarName = "Unknown";
            switch (holiday->calendar_type) {
                case CALENDAR_GREGORIAN: calendarName = "Gregorian"; break;
                case CALENDAR_ISLAMIC: calendarName = "Islamic"; break;
                case CALENDAR_CHINESE: calendarName = "Chinese"; break;
                case CALENDAR_HEBREW: calendarName = "Hebrew"; break;
            }
            [holidayInfo appendFormat:@"  Calendar: %s\n", calendarName];
            [holidayInfo appendFormat:@"  Description: %s\n\n", holiday->description];
        }
    } else {
        [holidayInfo appendString:@"No holidays loaded. Check data/holidays.csv file.\n"];
    }
    
    [self.holidayTextView setString:holidayInfo];
}

- (void)exportToCSV:(id)sender {
    NSSavePanel *savePanel = [NSSavePanel savePanel];
    [savePanel setNameFieldStringValue:@"calendar_export.csv"];
    
    // Simply use the deprecated API for compatibility - the warning is acceptable
    [savePanel setAllowedFileTypes:@[@"csv"]];
    
    [savePanel beginWithCompletionHandler:^(NSInteger result) {
        if (result == NSModalResponseOK) {
            NSURL *url = [savePanel URL];
            
            // Create export options
            ExportOptions options = {0};
            options.format = EXPORT_CSV;
            options.calendar_type = CALENDAR_GREGORIAN;
            
            // Get current year
            NSDate *now = [NSDate date];
            NSCalendar *gregorianCalendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
            NSDateComponents *components = [gregorianCalendar components:NSCalendarUnitYear fromDate:now];
            
            options.start_year = (int)[components year];
            options.end_year = (int)[components year];
            options.include_holidays = 1;
            options.include_moon_phases = 1;
            strcpy(options.country_code, "US");
            strcpy(options.filename, [[url path] UTF8String]);
            
            CalendarResult result = export_calendar_year(&options, options.start_year);
            
            dispatch_async(dispatch_get_main_queue(), ^{
                if (result == CALENDAR_SUCCESS) {
                    [self.exportStatusLabel setStringValue:[NSString stringWithFormat:@"Successfully exported to %@", [url lastPathComponent]]];
                } else {
                    [self.exportStatusLabel setStringValue:@"Export failed"];
                }
            });
        }
    }];
}

- (void)exportToICal:(id)sender {
    [self.exportStatusLabel setStringValue:@"iCal export not yet implemented"];
}

- (void)dealloc {
    if (holidayDB) {
        holiday_db_destroy(holidayDB);
    }
    // Note: In ARC (Automatic Reference Counting), [super dealloc] is not needed
}

@end