//
//  render.h
//  FakeDepth
//
//  Created by Simon Harvey on 11-12-04.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef FakeDepth_render_h
#define FakeDepth_render_h

#include <vector>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/EAGL.h>
#include "gl.h"
#include <GLKit/GLKMatrix4.h>

using namespace std;

namespace renderer {
	
	template <typename T>
	struct Point
	{
		T x, y;
	};
	
	template <typename T>
	struct Rect 
	{
		Point<T> a, b, c, d;
	};
	
	template <typename T>
	Rect<T> make_rect(T x, T y, T w, T h)
	{
		Rect<T> r = {
			{x, y},
			{x, y+h},
			{x+w, y+h},
			{x+w, y}
		};
		return r;
	}
	
	struct Sprite
	{
		GLuint colorTex, depthTex;
		Rect<float> rect;
		Rect<float> colorTexCoords;
		
		Sprite() 
		{
			
		}
	};
	
	struct Renderer
	{
		vector<Sprite> sprites;
		GLuint gl_program;
		GLuint vertexArray, vertexBuffer;
		
		Renderer(const char *vsh, const char *fsh)
		{
			glGenVertexArraysOES(1, &vertexArray);
			glBindVertexArrayOES(vertexArray);
			
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			
			gl_program = glCreateProgram();
			shaderAttachFromFile(gl_program, GL_VERTEX_SHADER, vsh);
			shaderAttachFromFile(gl_program, GL_FRAGMENT_SHADER, fsh);
			glLinkProgram(gl_program);
		}
		
		void render()
		{
			glClearColor(.2, .2, .2, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_BLEND);
			
			glUseProgram(gl_program);
			
			// setup projection matrix ortho
			
			/*static float projection_matrix[] = {
				2.0f/768.0f, 0, 0, -1,
				0, 2.0f/1024.0f, 0, -1,
				0, 0, -1, 0,
				0, 0, 0, 1
			};*/
			
			GLKMatrix4 proj = GLKMatrix4MakeOrtho(0, 768, 0, 1024, -1, 1000);
			
			GLuint proj_u = glGetUniformLocation(gl_program, "projection");
			GLuint pos_a = glGetAttribLocation(gl_program, "position");
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			for (Sprite &sp : sprites) {
				glEnableVertexAttribArray(pos_a);
				glVertexAttribPointer(pos_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.rect);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
		
		void add_sprite(Sprite sp)
		{
			sprites.push_back(sp);
		}
	};
}

#endif
