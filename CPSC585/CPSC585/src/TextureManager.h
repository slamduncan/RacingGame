#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "SDL_image.h"


class TextureManager
{
private:
	static TextureManager* instance;

	TextureManager();

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
	

	int loadTexture(std::string filename, int texID = -1);
	int loadTextureFromImage();

	void freeTexture();
	void freeAll();

	int getNumTex();
	int getTexID(int index);


};
#endif