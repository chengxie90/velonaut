// #include "ogretest.h"
#ifdef __APPLE__
#include "macosx.h"
#endif

#include "app.h"

int main(int argc, char *argv[]) {
    App app;
    app.init();
#ifdef __APPLE__
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    AppDelegate *appDelegate = [[AppDelegate alloc] init];
    appDelegate.app = &app;
    [[NSApplication sharedApplication] setDelegate:appDelegate];
	NSApplicationMain(argc, (const char **)argv);
    [pool release];
#endif
    return 0;
}

