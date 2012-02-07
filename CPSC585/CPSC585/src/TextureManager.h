#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"
#include "SDL_image.h"

class TextureManager
{
private:
	static TextureManager* instance;

	TextureManager();

	std::map<std::string, GLuint> textureMap;
	int numTextures;

public:
	static TextureManager* initialize()
	{
		if(instance == NULL)
		{
			instance = new TextureManager();
		}

		return instance;
	}
	

	int loadTexture(std::string filename, std::string name);
	int loadTextureFromImage();

	void freeTexture();
	void freeAll();

	int getNumTex();
	int getTexID(int index);


};
#endif