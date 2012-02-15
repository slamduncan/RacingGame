/*
*  Shader Header File
*
*  Name: Jeffrey Lam
*/

#ifndef SHADER_H
#define SHADER_H

#include "glew.h"
#include <iostream>

class Shader {
private:

	char* textFileRead(const char* filename);
	int textFileWrite(char* filename, char *s);

public:
	// v - vertex
	// f - fragment
	// p - program
	GLuint v, f, p;
	
	Shader();
	Shader(char* vert, char* frag);
	~Shader();

	GLint getUniform(const char* name); 

	void turnShadersOn();
	void turnShadersOff();

	void debug();

};

#endif
