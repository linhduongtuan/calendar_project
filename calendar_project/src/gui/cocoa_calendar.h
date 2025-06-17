// src/gui/cocoa_calendar.h (Fixed header)
#ifndef COCOA_CALENDAR_H
#define COCOA_CALENDAR_H

#import <Cocoa/Cocoa.h>

@interface CalendarController : NSViewController <NSApplicationDelegate>

@property (strong, nonatomic) NSDatePicker *datePicker;
@property (strong, nonatomic) NSTextView *conversionTextView;
@property (strong, nonatomic) NSTextView *moonPhaseTextView;
@property (strong, nonatomic) NSTextView *holidayTextView;
@property (strong, nonatomic) NSTextField *exportStatusLabel;

- (void)setupUI;
- (void)updateConversions;
- (void)dateChanged:(id)sender;
- (void)exportToCSV:(id)sender;
- (void)exportToICal:(id)sender;

@end

#endif // COCOA_CALENDAR_H