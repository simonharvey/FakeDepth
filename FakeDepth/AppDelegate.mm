//
//  AppDelegate.m
//  FakeDepth
//
//  Created by Simon Harvey on 11-12-04.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "render.h"

#import <GLKit/GLKit.h>
#import "GLViewController.h"

@implementation AppDelegate

@synthesize window = _window;

- (void)dealloc
{
	[_window release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
	self.window.backgroundColor = [UIColor whiteColor];
    
	GLViewController *controller = [[GLViewController alloc] init];
	self.window.rootViewController = controller;
	[self.window addSubview:controller.view];
	
	[self.window makeKeyAndVisible];
	
	return YES;
}

@end
