#include "FBO.h"

FBO::FBO()
{
}

FBO::~FBO()
{
}

void FBO::init(int width, int height)
{
	glGenFramebuffersEXT(1, &fb);	// generate the FBO
	turnOn();	// bind the FBO to initialize data for it

	glGenRenderbuffersEXT(1, &rb);	// generate the render buffer

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rb);	// bind it

	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, rb);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);	// unbind the render buffer

	turnOff();	// unbind it to resume normal frame buffer
}

/*
*	types: 
*	color - GL_COLOR_ATTACHMENT0_EXT
*	depth - GL_DEPTH_ATTACHMENT_EXT
*/
void FBO::attachTexture(GLuint texture, GLenum type)
{
/*	
	if(type > GL_MAX_COLOR_ATTACHMENTS_EXT)
	{
		printf("over max color attachment point\n");
		printf("%d, %d\n",GL_MAX_COLOR_ATTACHMENTS_EXT, type);	
		
		return;
	}
*/	
	turnOn();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
	turnOff();
}

void FBO::deattachTexture()
{
	turnOn();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0);
	turnOff();
}

bool FBO::isValid()
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		return true;
	}
	return false;
}

void FBO::turnOn()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
}

void FBO::turnOff()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}