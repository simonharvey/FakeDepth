//
//  gl.h
//  CPP11
//
//  Created by Simon Harvey on 11-12-02.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CPP11_gl_h
#define CPP11_gl_h

#include <OpenGLES/es2/gl.h>
#include <memory.h>
#include <iostream>

using namespace std;

// TODO: return a smart pointer to the data so the caller doesnt have to free the result.
static char *
shaderLoadSource(const char *filePath)
{
	FILE * fp = fopen(filePath, "rb");
	if (fp == NULL) return 0;
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *pData = new char[size + 1];
	//memset(pData, '?', size);
	int lRet = fread(pData, sizeof(char), size, fp);
	fclose(fp);
	pData[size] = NULL;
	//cout << "----" << pData << "----" << endl;
	return pData;
}

/*
 * Returns a shader object containing a shader
 * compiled from the given GLSL shader file.
 */
static GLuint
shaderCompileFromFile(GLenum type, const char *filePath)
{
	char *source;
	GLuint shader;
	GLint length, result;
	
	/* get shader source */
	source = shaderLoadSource(filePath);
	if(!source)
		return 0;
	
	/* create shader object, set the source, and compile */
	shader = glCreateShader(type);
	length = strlen(source);
	glShaderSource(shader, 1, (const char **)&source, &length);
	glCompileShader(shader);
	free(source);
	
	/* make sure the compilation was successful */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		char *log;
		
		/* get the shader info log */
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		log = (char *)malloc(length);
		glGetShaderInfoLog(shader, length, &result, log);
		
		/* print an error message and the info log */
		fprintf(stderr, "shaderCompileFromFile(): Unable to compile %s: %s\n", filePath, log);
		free(log);
		
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;
}

/*
 * Compiles and attaches a shader of the
 * given type to the given program object.
 */
static void
shaderAttachFromFile(GLuint program, GLenum type, const char *filePath)
{
	/* compile the shader */
	GLuint shader = shaderCompileFromFile(type, filePath);
	if(shader != 0) {
		/* attach the shader to the program */
		glAttachShader(program, shader);
		
		/* delete the shader - it won't actually be
		 * destroyed until the program that it's attached
		 * to has been destroyed */
		glDeleteShader(shader);
	} else {
		std::cout << "could not attach shader: " << filePath << endl;
	}
}

#endif
