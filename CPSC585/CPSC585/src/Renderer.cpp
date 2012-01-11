#include "Renderer.h"

Renderer::Renderer()
{
	info = NULL;
	width = 1280;
	height = 720;
	bpp = 0;
}

Renderer::Renderer(int w, int h)
{
	info = NULL;
	width = w;
	height = h;
	bpp = 0;
}

Renderer::~Renderer()
{
}


/*
*	initializes SDL window
*
*/
void Renderer::initSDL()
{
	//SDL_Init( SDL_INIT_EVERYTHING );
	
	// if the video does not initialize
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        quitSDL();
    }

	info = SDL_GetVideoInfo();

	if(!info) {
        /* This should probably never happen. */
        fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
        quitSDL();
    }

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	vflags = SDL_OPENGL;	// set the video flags to allow opengl rendering

	bpp = info->vfmt->BitsPerPixel;	// get the bits per pixel of the screen

	// Try to set the window up
	if( SDL_SetVideoMode( width, height, bpp, vflags ) == 0 ) {
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError() );
        quitSDL();
    }
	
	// Window created, and nothing went wrong
}

/*
*	NEED TO FIX, PRIVATE MEMBER VARABLES FOR W AND H
*/
void Renderer::initGL(int w, int h)
{
	if(w <= 0 || h <= 0)
	{
		fprintf( stderr, "Resolution not set properly, exiting...: %s\n", SDL_GetError() );
        quitSDL();
	}
	
	float ratio = (float) w / (float) h;

    glShadeModel( GL_SMOOTH );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    glClearColor( 0, 0, 0, 0 );

    glViewport( 0, 0, w, h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    gluPerspective( 60.0, ratio, 1.0, 1024.0 );

	glMatrixMode(GL_MODELVIEW);	// switch back to model view
}

/*
*	Closes SDL/window and quits the program?
*
*/
void Renderer::quitSDL()
{
	SDL_Quit();

	printf("Closing program...\n");

	exit(0);
}

/*
*	draws a white box centered on the screen
*/
void Renderer::draw()
{
	glPushMatrix();
    glLoadIdentity();

	gluLookAt(0, 0, -5,	// camera position
		      0, 0, 0,	// look at point
			  0, 1, 0);	// up vector
		      
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);
	glEnd();

	glPopMatrix();

}

/*
*	Draws a given entity to the screen
*
*	Parameters:
*/
void Renderer::drawEntity()
{
	
}

/*
*	Clears the frame buffer
*/
void Renderer::clearGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
*	flush to frame buffer
*/
void Renderer::updateGL()
{
	SDL_GL_SwapBuffers();
}
