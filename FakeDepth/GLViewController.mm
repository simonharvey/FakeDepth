//
//  GLViewController.m
//  FakeDepth
//
//  Created by Simon Harvey on 11-12-04.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "GLViewController.h"

@implementation GLViewController

#pragma mark - View lifecycle

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
	NSLog(@"load view");
	GLKView *v = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	v.delegate = self;
	self.view = v;
	[v release];
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
	NSLog(@"view did load");
	EAGLContext *c = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	[EAGLContext setCurrentContext:c];
	((GLKView *)self.view).context = c;
	const char * vsh = [[[NSBundle mainBundle] pathForResource:@"fake_depth" ofType:@"vsh"] UTF8String];
	const char * fsh = [[[NSBundle mainBundle] pathForResource:@"fake_depth" ofType:@"fsh"] UTF8String];
	
	NSString *path = [[NSBundle mainBundle] pathForResource:@"skel" ofType:@"jpg"];
	GLKTextureInfo *tex_info = [GLKTextureLoader textureWithContentsOfFile:path options:nil error:nil];
	
	_renderer = new Renderer(vsh, fsh);
	for (int i=0; i<10; i++) {
		Sprite sp;
		sp.rect = make_rect<float>(50 * i, 50 * i, 100, 100);
		sp.colorTexCoords = make_rect<float>(0, 0, 1, 1);
		sp.colorTex = tex_info.name;
		_renderer->add_sprite(sp);
	}
}

//

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect 
{
	_renderer->render();
	//NSLog(@"Draw in rect");
}

@end
