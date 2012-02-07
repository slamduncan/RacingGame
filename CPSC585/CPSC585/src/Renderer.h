#ifndef RENDERER_H
#define RENDERER_H

//#define NDEBUG
#include <assert.h>

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include "Shader.h"
#include "Entity.h"
#include "TextureManager.h"

#include "SDL.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "SDL_image.h"


#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btIDebugDraw.h"

class Renderer : public btIDebugDraw
{
private:
	static Renderer *instance;
	TextureManager* tm;

	Renderer();	
	
	int m_debugMode;

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

	SDL_Surface* loadIMG(std::string filename);
	GLuint initTexture(SDL_Surface* image);
	void textureOn(GLuint texID);
	void textureOff();
	void framebufferOn(GLuint fbID);
	void framebufferOff();


	bool init();
	int initSDL();
	int initGL();
	int initFont();

	void outputText(std::string text, int r, int g, int b, int x, int y);

	void setCamera(const btVector3& pos, const btVector3& lookAt); 

	void draw();
	void drawEntity(Entity &entity);

	void drawPlane(float height);
	void drawLine(btVector3 &start, btVector3 &end, int r, int g, int b, float width = 1.0);

	void glEnable2D();
	void glDisable2D();
	
	void glEnableLighting();
	void glDisableLighting();

	void updateGL();
	void clearGL();

	void quitSDL();



	//Following functions for bullet debug. API is here http://bulletphysics.com/Bullet/BulletFull/classbtIDebugDraw.html	
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	void reportErrorWarning(const char *warningString);
	void draw3dText(const btVector3 &location, const char *textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;

};

#endif
