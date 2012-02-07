#include "TextureManager.h"

TextureManager* TextureManager::instance = 0;

TextureManager::TextureManager()
{
	numTextures = 0;
}

int TextureManager::loadTexture(std::string filename, std::string name)
{
	std::ifstream fileCheck(filename.c_str());

	// check and see if the file exists
	if(fileCheck)
	{
		// load the image into SDL surface
		SDL_Surface* image = IMG_Load(filename.c_str());
		
		GLuint texID = 0;
		GLenum texType;

		GLint nChannel = image->format->BytesPerPixel;
		
		if (nChannel == 4)     // contains an alpha channel
		{
				if (image->format->Rmask == 0x000000ff)
				{
						texType = GL_RGBA;
				}
				else
				{
						texType = GL_BGRA;
				}
		} 
		else if (nChannel == 3)     // no alpha channel
		{
				if (image->format->Rmask == 0x000000ff)
				{
					texType = GL_RGB;
				}
				else
				{
					texType = GL_BGR;
				
				}
		} 
		else 
		{
				printf("warning: the image is not truecolor..  this will probably break\n");
				// this error should not go unhandled
		}

		glGenTextures(1, &texID);	// generate texture id
		glBindTexture(GL_TEXTURE_2D, texID);	// bind our texture to our texture id
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexImage2D(GL_TEXTURE_2D, 0, nChannel, image->w, image->h, 0, texType, GL_UNSIGNED_BYTE, image->pixels);
		glBindTexture(GL_TEXTURE_2D, 0);  // unbind our texture

		//textureMap.insert(name, texID);


		// free up the image since we already have it in GPU memory
		if(image)
		{ 
			SDL_FreeSurface(image);
		}
	}
	
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
