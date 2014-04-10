//
//  macosx.h
//  velonaut
//
//  Created by Cheng Xie on 4/8/14.
//
//

#include <Cocoa/Cocoa.h>
#include "app.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (retain) NSTimer *timer;
@property (nonatomic) App *app;

- (void)start;
- (void)renderOneFrame:(id)sender;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)application
{
    [self start];
    //[[NSApplication sharedApplication] terminate:nil];
}

- (void)start
{
    self.timer = [[NSTimer timerWithTimeInterval: 0.001 target:self selector:@selector(renderOneFrame:) userInfo:self repeats:true] retain];
	[[NSRunLoop currentRunLoop] addTimer:self.timer forMode: NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:self.timer forMode: NSEventTrackingRunLoopMode];
}

- (void)renderOneFrame:(id)sender
{
    if (self.app) {
        self.app->render();
    }
}

@end


