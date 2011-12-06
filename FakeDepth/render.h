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
		GLuint depth_glprog, color_glprog;
		GLuint depth_fbo;
		//GLuint vertexArray, vertexBuffer;
		
		Renderer(const char *vsh, const char *fsh)
		{
			// create the FBO
			glGenFramebuffers(1, &depth_fbo);
			
			depth_glprog = glCreateProgram();
			shaderAttachFromFile(depth_glprog, GL_VERTEX_SHADER, abs_path("fake_depth.vsh"));
			shaderAttachFromFile(depth_glprog, GL_FRAGMENT_SHADER, abs_path("fake_depth.fsh"));
			glLinkProgram(depth_glprog);
			
			color_glprog = glCreateProgram();
			shaderAttachFromFile(color_glprog, GL_VERTEX_SHADER, abs_path("fake_color.vsh"));
			shaderAttachFromFile(color_glprog, GL_FRAGMENT_SHADER, abs_path("fake_color.fsh"));
			glLinkProgram(color_glprog);
			
			GLuint vertexBuffer;
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			
			
			/*glGenVertexArraysOES(1, &vertexArray);
			glBindVertexArrayOES(vertexArray);
			
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			
			gl_program = glCreateProgram();
			shaderAttachFromFile(gl_program, GL_VERTEX_SHADER, vsh);
			shaderAttachFromFile(gl_program, GL_FRAGMENT_SHADER, fsh);
			glLinkProgram(gl_program);*/
		}
		
		void depth_render_pass()
		{
			GLint old_fbo;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
			
			GLuint gl_program = depth_glprog;
			
			glUseProgram(gl_program);
			
			GLKMatrix4 proj = GLKMatrix4MakeOrtho(0, 768, 0, 1024, -1, 1000);
			
			const GLuint proj_u = glGetUniformLocation(gl_program, "projection");
			const GLuint pos_a = glGetAttribLocation(gl_program, "position");
			const GLuint ctex_coords_a = glGetAttribLocation(gl_program, "texCoords");
			const GLuint ctex_u = glGetUniformLocation(gl_program, "depthTex");
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			// todo: keep the sprites sorted by texture to prevent most glBindTexture calls.
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
			
			glBindFramebuffer(GL_FRAMEBUFFER, old_fbo);
		}
		
		void color_render_pass()
		{
			GLuint gl_program = color_glprog;
			
			glUseProgram(gl_program);
			
			GLKMatrix4 proj = GLKMatrix4MakeOrtho(0, 768, 0, 1024, -1, 1000);
			
			const GLuint proj_u = glGetUniformLocation(gl_program, "projection");
			const GLuint pos_a = glGetAttribLocation(gl_program, "position");
			const GLuint ctex_coords_a = glGetAttribLocation(gl_program, "texCoords");
			const GLuint ctex_u = glGetUniformLocation(gl_program, "colorTex");
			const GLuint dtex_u = glGetUniformLocation(gl_program, "depthTex");
			
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			// todo: keep the sprites sorted by texture to prevent most glBindTexture calls.
			for (Sprite &sp : sprites) {
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sp.colorTex);
				glUniform1i(ctex_u, 0);
				
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, sp.depthTex);
				glUniform1i(dtex_u, 1);
				
				glEnableVertexAttribArray(pos_a);
				glVertexAttribPointer(pos_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.rect);
				
				glEnableVertexAttribArray(ctex_coords_a);
				glVertexAttribPointer(ctex_coords_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.colorTexCoords);
				
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
			
		}
		
		void render()
		{
			glClearColor(.2, .2, .2, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_BLEND);
			
			//depth_render_pass();
			color_render_pass();
			
			/*glUseProgram(gl_program);
			
			GLKMatrix4 proj = GLKMatrix4MakeOrtho(0, 768, 0, 1024, -1, 1000);
			
			const GLuint proj_u = glGetUniformLocation(gl_program, "projection");
			const GLuint pos_a = glGetAttribLocation(gl_program, "position");
			const GLuint ctex_coords_a = glGetAttribLocation(gl_program, "colorTexCoords");
			const GLuint ctex_u = glGetUniformLocation(gl_program, "colorTex");
			const GLuint dtex_u = glGetUniformLocation(gl_program, "depthTex");
			
			glUniformMatrix4fv(proj_u, 1, GL_FALSE, proj.m);
			
			// todo: keep the sprites sorted by texture to prevent most glBindTexture calls.
			for (Sprite &sp : sprites) {
				
				glEnableVertexAttribArray(pos_a);
				glVertexAttribPointer(pos_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.rect);
				
				glActiveTexture(0);
				glBindTexture(GL_TEXTURE_2D, sp.colorTex);
				glUniform1i(ctex_u, 0);
				
				glActiveTexture(1);
				glBindTexture(GL_TEXTURE_2D, sp.depthTex);
				glUniform1i(dtex_u, 1);
				
				glEnableVertexAttribArray(ctex_coords_a);
				glVertexAttribPointer(ctex_coords_a, 2, GL_FLOAT, GL_FALSE, 0, &sp.colorTexCoords);
				
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}*/
		}
		
		void add_sprite(Sprite sp)
		{
			sprites.push_back(sp);
		}
	};
}

#endif
