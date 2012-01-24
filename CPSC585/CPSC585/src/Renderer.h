#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include "Shader.h"
#include "Entity.h"

#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "SDL_image.h"


using namespace std;

class Renderer
{
private:
	static Renderer *instance;
	
	Renderer();	
	
	int width;
	int height;
	int bpp;
	int vflags;

	TTF_Font *debugFont;

	const SDL_VideoInfo* info;	// pointer to video info for screen

	Shader shader;	// shader for drawing

public:

	~Renderer();

	static Renderer* getInstance()
	{
		if (instance == 0)
		{
			instance = new Renderer();
		}
		return instance;
	}

	SDL_Surface* loadIMG(string filename);
	GLuint initTexture(SDL_Surface* image);
	void textureOn(GLuint texID);
	void textureOff();
	void framebufferOn(GLuint fbID);
	void framebufferOff();

	void initSDL();
	void initGL(int width, int height);
	void initFont();

	void outputText(string text, int r, int g, int b, int x, int y);

	void setCamera(const btVector3& pos, const btVector3& lookAt); 

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
