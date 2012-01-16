#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include "Shader.h"
#include "Entity.h"

#include "../include/SDL/SDL.h"
#define NO_SDL_GLEXT
#include "../include/SDL/SDL_opengl.h"
#include "../include/SDL/SDL_ttf.h"


using namespace std;

class Renderer
{
private:
	int width;
	int height;
	int bpp;
	int vflags;

	TTF_Font *debugFont;

	const SDL_VideoInfo* info;	// pointer to video info for screen

	Shader shader;	// shader for drawing

public:
	Renderer();
	Renderer(int w, int h);
	~Renderer();

	void initSDL();
	void initGL(int width, int height);
	void initFont();

	void outputText(string text, int r, int g, int b, int x, int y);

	void draw();
	void drawEntity(Entity &entity);

	void drawPlane(float height);

	void glEnable2D();
	void glDisable2D();

	void updateGL();
	void clearGL();

	void quitSDL();

};

#endif
