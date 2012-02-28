#ifndef FBO_H
#define FBO_H

#include "glew.h"
#include <stdio.h>

class FBO
{
private:
	// frame buffer and render buffer
	GLuint fb, rb;

public:
	FBO();
	~FBO();

	void init(int width, int height);
	void attachTexture(GLuint texture, GLenum type);
	void deattachTexture();

	bool isValid();

	void turnOn();
	void turnOff();
};

#endif