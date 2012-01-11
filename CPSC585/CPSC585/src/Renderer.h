#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"

#include "../include/SDL.h"
#define NO_SDL_GLEXT
#include "../include/SDL_opengl.h"
#include "../include/SDL_ttf.h"

class Renderer
{
private:
	int width;
	int height;
	int bpp;
	int vflags;

	const SDL_VideoInfo* info;	// pointer to video info for screen

	Shader shader;	// shader for drawing


public:
	Renderer();
	Renderer(int w, int h);
	~Renderer();

	void initSDL();
	void initGL(int width, int height);

	void draw();
	void drawEntity();
	void updateGL();
	void clearGL();

	void quitSDL();

};

#endif
