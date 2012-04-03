#include "TextureManager.h"

using namespace std;

TextureManager* TextureManager::instance = 0;

TextureManager::TextureManager()
{
	numTextures = 0;
}

TextureManager::~TextureManager()
{
	freeAll();
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
		
		texman.insert(pair<string, int>(name, texID));

		keys.push_back(name);

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

GLuint TextureManager::genTexture(std::string filename, std::string key)
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
		
		texman.insert(pair<string, GLuint>(key, texID));
		keys.push_back(key);

		// free up the image since we already have it in GPU memory
		if(image)
		{ 
			SDL_FreeSurface(image);
		}

		return texID;
	}
	return 0;
}
GLuint TextureManager::genTexture(unsigned int width, unsigned int height, std::string key)
{
	GLuint texID = 0;

	glGenTextures(1, &texID);	// generate a space in gpu memory
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);	// generate 32bit precision floats for each channel
	glBindTexture(GL_TEXTURE_2D, 0);  // unbind out texture

	texman.insert(pair<string, GLuint>(key, texID));
	keys.push_back(key);

	return texID;
}
	

void TextureManager::freeTexture()
{

}

void TextureManager::freeAll()
{
	for(unsigned int i = 0; i < keys.size(); i++)
	{
		GLuint id = getTexture(keys[i]);
		glDeleteTextures(1, &id);
	}
	keys.clear();
	texman.clear();
}

int TextureManager::getNumTex()
{
	return texman.size();
}

int TextureManager::getTexID(int index)
{
	return 0;
}

GLuint TextureManager::getTexture(std::string key)
{
	int counter = texman.count(key);
	
	if(counter > 0)
	{	
		return texman.find(key)->second;
	}
	else
	{
		return 0;
	}
}
