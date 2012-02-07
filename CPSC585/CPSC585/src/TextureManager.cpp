#include "TextureManager.h"

TextureManager* TextureManager::instance = 0;

TextureManager::TextureManager()
{
	numTextures = 0;
}

int TextureManager::loadTexture(std::string filename, int texID)
{
	return 0;
}


int TextureManager::loadTextureFromImage()
{
	return 0;
}

void TextureManager::freeTexture()
{

}

void TextureManager::freeAll()
{

}

int TextureManager::getNumTex()
{
	return numTextures;
}

int TextureManager::getTexID(int index)
{
	return 0;
}
