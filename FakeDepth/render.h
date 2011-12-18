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
#include <OpenGLES/ES2/glext.h>
#include "gl.h"
#include <GLKit/GLKMatrix4.h>

using namespace std;

inline const char * abs_path(const char *f)
{
	NSString *s = [NSString stringWithUTF8String:f];
	NSArray *a = [s componentsSeparatedByString:@"."];
	NSString *name = [a objectAtIndex:0];
	NSString *type = [a objectAtIndex:1];
	NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:type];	  
	return [path UTF8String];
}

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
		T width() { return d.x - a.x; }
		T height() { return b.y - a.y; }
		T x() { return a.x; }
		T y() { return a.y; }
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
	};
	
	struct RenderTexture
	{
		GLuint framebuffer;
		GLuint tex;
		GLint old_fbo;
		const GLuint width, height;
		
		RenderTexture(GLuint width, GLuint height) : width(width), height(height)
		{
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
			
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			
			void *data = malloc((int)(height	* height * 4));
			memset(data, 0, (int)(height * height * 4));
			
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, height, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
			
			/*
			GLuint depthRenderbuffer;
			glGenRenderbuffers(1, &depthRenderbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
			*/
			 
			GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				cout << status << endl;
				throw new exception();
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		}
		
		void begin()
		{
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, width, height);
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		
		void end()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		}
	};
	
	struct Renderer
	{
		vector<Sprite> sprites;
		GLuint depth_glprog;
		RenderTexture * depth_off;
		static const GLuint size_x = 768, size_y = 1024;
		GLKMatrix4 proj;
		
		Renderer()
		{
			proj = GLKMatrix4MakeOrtho(0, size_x, size_y, 0, 0.0f, 1000.0f);
			
			depth_off = new RenderTexture(size_x, size_y);
			depth_glprog = glCreateProgram();
			shaderAttachFromFile(depth_glprog, GL_VERTEX_SHADER, abs_path("simple_texture.vsh"));
			shaderAttachFromFile(depth_glprog, GL_FRAGMENT_SHADER, abs_path("simple_texture.fsh"));
			glLinkProgram(depth_glprog);
			
			GLuint vertexBuffer;
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		}
		
		void depth_render_pass()
		{
			GLuint gl_program = depth_glprog;
			glUseProgram(gl_program);
			
			const GLuint proj_u =			glGetUniformLocation(gl_program, "projection");
			const GLuint pos_a =			glGetAttribLocation	(gl_program, "position");
			const GLuint ctex_coords_a =	glGetAttribLocation	(gl_program, "texCoords");
			const GLuint ctex_u =			glGetUniformLocation(gl_program, "tex");
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			for (Sprite &sp : sprites) {
				glEnableVertexAttribArray(pos_a);
				glVertexAttribPointer(pos_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.rect);
				
				glActiveTexture(0);
				glBindTexture(GL_TEXTURE_2D, sp.depthTex);
				glUniform1i(ctex_u, 0);
				
				glEnableVertexAttribArray(ctex_coords_a);
				glVertexAttribPointer(ctex_coords_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.colorTexCoords);
				
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
		
		void debug_depth_render_pass()
		{
			GLuint gl_program = depth_glprog;
			glUseProgram(gl_program);
			
			const GLuint proj_u =			glGetUniformLocation(gl_program, "projection");
			const GLuint pos_a =			glGetAttribLocation	(gl_program, "position");
			const GLuint ctex_coords_a =	glGetAttribLocation	(gl_program, "texCoords");
			const GLuint ctex_u =			glGetUniformLocation(gl_program, "tex");
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			Rect<float> rect = make_rect<float>(0, 0, size_x/3, size_y/3);
			glEnableVertexAttribArray(pos_a);
			glVertexAttribPointer(pos_a, 2, GL_FLOAT, GL_FALSE, 0, &rect);
			
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depth_off->tex);
			//glBindTexture(GL_TEXTURE_2D, sprites[0].colorTex); // uncomment this and the texture shows up, so not a problem with the shader...
			glUniform1i(ctex_u, 2);
			
			glEnableVertexAttribArray(ctex_coords_a);
			Rect<float> tex_coords = make_rect<float>(0, 0, 1, 1);
			glVertexAttribPointer(ctex_coords_a, 2, GL_FLOAT, GL_FALSE, 0, &tex_coords);
			
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		
		void render()
		{
			glClearColor(.2, .2, .2, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			depth_off->begin();
			depth_render_pass();
			depth_off->end();
			
			depth_render_pass();
			debug_depth_render_pass();
		}
		
		void add_sprite(Sprite sp)
		{
			sprites.push_back(sp);
		}
	};
}

#endif
