#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <utility>
#include "glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"

typedef std::map<std::string, GLuint, std::less<std::string>> TextureMap;

class TextureManager
{
private:
	static TextureManager* instance;

	TextureManager();

	TextureMap texman;


	int numTextures;

public:
	static TextureManager* getInstance()
	{
		if(instance == NULL)
		{
			instance = new TextureManager();
		}

		return instance;
	}

	enum IDTYPE
	{
		TEXTURE = 0,
		FBO,
		VBO
	};


	int loadTexture(std::string filename, std::string name);
	GLuint genTexture(std::string filename, std::string key);
	GLuint genTexture(unsigned int width, unsigned int height, std::string key);
	
	int loadTextureFromImage();

	void freeTexture();
	void freeAll();

	int getNumTex();
	int getTexID(int index);

	GLuint getTexture(std::string key);

};
#endif