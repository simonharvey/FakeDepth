//
//  GLViewController.h
//  FakeDepth
//
//  Created by Simon Harvey on 11-12-04.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "render.h"

using namespace renderer;

@interface GLViewController : GLKViewController {

	Renderer *_renderer;
}

@end
