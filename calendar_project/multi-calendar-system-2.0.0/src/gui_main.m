// src/gui_main.m (Fixed GUI main function)
#import <Cocoa/Cocoa.h>
#import "gui/cocoa_calendar.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Create the application
        NSApplication *app = [NSApplication sharedApplication];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        // Create the main window
        NSRect windowFrame = NSMakeRect(100, 100, 800, 600);
        NSWindow *window = [[NSWindow alloc] 
            initWithContentRect:windowFrame
            styleMask:(NSWindowStyleMaskTitled | 
                      NSWindowStyleMaskClosable | 
                      NSWindowStyleMaskResizable | 
                      NSWindowStyleMaskMiniaturizable)
            backing:NSBackingStoreBuffered
            defer:NO];
        
        [window setTitle:@"Multi-Calendar System v2.0"];
        [window center];
        
        // Create the calendar controller
        CalendarController *calendarController = [[CalendarController alloc] init];
        
        // Set up the view controller
        [window setContentViewController:calendarController];
        
        // Set the app delegate
        [app setDelegate:calendarController];
        
        // Show the window
        [window makeKeyAndOrderFront:nil];
        
        // Bring app to front
        [app activateIgnoringOtherApps:YES];
        
        // Start the application run loop
        [app run];
    }
    
    return 0;
}