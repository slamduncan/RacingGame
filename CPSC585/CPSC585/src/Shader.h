/*
*  Shader Header File
*
*  Name: Jeffrey Lam
*/

#ifndef SHADER_H
#define SHADER_H

#include "glew.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <iostream>

struct Param
{
	enum ParamType
	{
		UNIFORM = 0,
		ATTRIBUTE,
	};
	std::string name;
	ParamType type;
	int size;
	GLuint location;
};

class Shader {
private:

	char* textFileRead(const char* filename);
	int textFileWrite(char* filename, char *s);
	std::vector<Param> paramList;

public:
	// v - vertex
	// f - fragment
	// p - program
	GLuint v, f, p;
	
	Shader();
	Shader(const char* vert,const char* frag);
	~Shader();

	GLint getUniform(const char* name); 

	void turnShadersOn();
	void turnShadersOff();

	void initParam(std::vector<std::string> paramName, std::vector<float> paramValues); 

	void debug();

};

#endif
