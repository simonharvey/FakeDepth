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

- (void)loadView
{
	GLKView *v = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	v.delegate = self;
	self.view = v;
	[v release];
}

#define LOAD_TEX(name, type)\
[GLKTextureLoader textureWithContentsOfFile:[[NSBundle mainBundle] pathForResource:name ofType:type] options:nil error:nil]

- (void)viewDidLoad
{
    [super viewDidLoad];
	NSLog(@"view did load");
	EAGLContext *c = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	[EAGLContext setCurrentContext:c];
	GLKView *glk = ((GLKView *)self.view);
	glk.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
	glk.drawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
	glk.context = c;
	
	_renderer = new Renderer();
	
	Sprite fg;
	fg.rect = make_rect<float>(0, 0, 640, 480);
	fg.colorTexCoords = make_rect<float>(0, 0, 1, 1);
	fg.colorTex = LOAD_TEX(@"cube_color", @"jpg").name;
	fg.depthTex = LOAD_TEX(@"cube_depth", @"jpg").name;
	_renderer->add_sprite(fg);
}

-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect 
{
	_renderer->render();
}

@end
