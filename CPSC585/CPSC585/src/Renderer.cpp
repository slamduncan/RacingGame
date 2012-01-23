#include "Renderer.h"

Renderer* Renderer::instance = 0;


Renderer::Renderer()
{
	info = NULL;
	width = 1280;
	height = 720;
	bpp = 0;
}

// clean up
Renderer::~Renderer()
{
	TTF_CloseFont(debugFont);
	TTF_Quit();
}


/*
*	initializes SDL window
*/
void Renderer::initSDL()
{
	//SDL_Init( SDL_INIT_EVERYTHING );
	
	// if the video does not tempize
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

	// 8 bits for red, green blue, 32 bit depth
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
	
	float ratio = (float) w / (float) h;	// compute FOV

    
	//
	//	GL options go here, ie glEnable, etc
	//
	
	glShadeModel( GL_SMOOTH );	// smooth shading
    //glCullFace( GL_BACK );	// remove back facing surfaces
    //glFrontFace( GL_CCW );	// set front face objects to be in CCW direction
    //glEnable( GL_CULL_FACE );	// allow removing culled surfaces
	glBlendFunc(GL_ONE, GL_ONE);	
	//glEnable(GL_BLEND);


	GLfloat whiteDir[4] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lightPos[4] = {5, 5, 0, 0};


	// enable lighting
	glEnable(GL_LIGHTING);
	// enable light 0
    glEnable(GL_LIGHT0);
    
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
    glMaterialf(GL_FRONT, GL_SHININESS, 30.0);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);         // set the diffuse color for the light
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);        // set the specular color of the light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);        // set the position of the light


	glEnable( GL_COLOR_MATERIAL );						// allow shading for colored material


	GLenum err = glewInit();	// initialize GLEW
    if (GLEW_OK == err)
    {
        std::cout << "GLEW initialized" << std::endl;
    }

	
	glClearColor( 0, 0, 0, 0 );	// clear screen to black

    glViewport( 0, 0, w, h );	// set the viewport to be the resolution of the screen

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );	// need to fix this to change fov on the fly
	glMatrixMode(GL_MODELVIEW);	// switch back to model view
}

/*
*	initializes the font used.
*/
void Renderer::initFont()
{
	if(TTF_Init())
	{
		fprintf(stderr, "Font failed to initialize: %s\n", TTF_GetError());
        quitSDL();
	}

	debugFont = TTF_OpenFont("../CPSC585/font/ARIAL.TTF", 20);

	if(debugFont == NULL)
	{
		fprintf(stderr, "Font failed to load\n", 0);
        quitSDL();
	}
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
*	Outputs text to the screen using textures
*	Allows multiline output (ie. \n is supported);
*
*	Parameters:
*	text - the string we want to output to the screen
*	r, g, b - color of the text (0-255)
*	x, y - position of the text (Note: (0, 0) is on the bottom left corner of the screen)
*/
void Renderer::outputText(string text, int r, int g, int b, int x, int y)
{
	// early bailout, we only draw if there is text to draw
	if(text.length() == 0)
	{
		return;
	}
	
	vector<string> mlines;
	SDL_Surface *temp;	
	SDL_Surface *toTexture;
	SDL_Color color;
	SDL_Rect position;
	int wt = 0;
	int ht = 0;
	GLuint texture;
	int n = 0;

	color.r = r;
	color.g = g;
	color.b = b;

	position.x = x;
	position.y = y;

	// loop for all '\n' characters in a string
	while (n !=-1)
	{
		string subLine;

		n = text.find('\n', 0);	// find the location of the first '\n'

		subLine = text.substr(0, n);	// get the substring

		// if there is still text left
		if(n != -1)
		{
			text = text.substr(n+1, -1);	// advance the text to after the '\n' character
		}

		mlines.push_back(subLine);	// store the substring

		int w = 0;
		
		TTF_SizeText(debugFont, subLine.c_str(), &w, &ht);	// compute the width in pixels of the line
		
		// if the width computed for the line is bigger than the current width of the texture
		if(w > wt) 
		{  
			wt = w;  // set the current with of the texture
		}

	}

	int lineSkip = TTF_FontLineSkip(debugFont);	// get the number of pixels for the next line
	ht = (int)mlines.size() * lineSkip;	// compute the maximum height of the texture

	// generate a surface based on the width and height of the text
	toTexture = SDL_CreateRGBSurface(0, wt, ht, 32, 0, 0, 0, 0);

	// render each line into a surface
	for(int i = 0; i < (int)mlines.size(); i++)
	{
		temp = TTF_RenderText_Solid(debugFont, mlines[i].c_str(), color);	// render a line to the surface

		// compute the location of the next text location
		SDL_Rect skip;
		skip.x = 0;
		skip.y = i*lineSkip;

		// blit temp surface to toTexture surface based on the offset amount
		SDL_BlitSurface(temp, 0, toTexture, &skip);

		SDL_FreeSurface(temp);	// free the surface
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, wt, ht, 0, GL_BGRA, GL_UNSIGNED_BYTE, toTexture->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	/* Draw a quad at location */
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f); 
	glVertex2i(position.x, position.y);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex2i(position.x + wt, position.y);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex2i(position.x + wt, position.y + ht);
	glTexCoord2f(0.0f, 0.0f); 
	glVertex2i(position.x, position.y + ht);
	glEnd();

	glFinish();

	SDL_FreeSurface(toTexture);

	glDeleteTextures(1, &texture);

}

void Renderer::setCamera(const btVector3& pos, const btVector3& lookAtPoint)
{
	//glPushMatrix();
	glLoadIdentity();

	btVector3 up(0, 1, 0);
	btVector3 lookAtVector = (lookAtPoint - pos).normalize();

	btVector3 binormal = lookAtVector.cross(up);

	btVector3 normal = (binormal.cross(lookAtVector)).normalize();
/*	
	printf("campos: (%f, %f, %f)\n", pos.x(), pos.y(), pos.z());
	printf("camLookAt: (%f, %f, %f)\n", lookAtPoint.x(), lookAtPoint.y(), lookAtPoint.z()); 
	printf("camnormal: (%f, %f, %f)\n", normal.x(), normal.y(), normal.z()); 
*/

	gluLookAt(pos.x(), pos.y(), pos.z(),	// camera position
		      lookAtPoint.x(), lookAtPoint.y(), lookAtPoint.z(),	// look at point
			  normal.x(), normal.y(), normal.z());	// up vector

	//glPopMatrix();
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
		      
	glColor4f(1, 0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);
	glEnd();
	glPopMatrix();

	// go into HUD mode
	glEnable2D();

	outputText("This is a multi\nline test to see if \nnewlines are working correctly", 255, 255, 255, width/3, height/2);

	glDisable2D();
}

/*
*	Draws a given entity to the screen
*
*	Parameters:
*	entity - the entity we want to draw
*/
void Renderer::drawEntity(Entity &entity)
{
	objLoader* renderObject = entity.renderObject;
	
	btVector3 p, t, n, b;

	p = entity.position;
	t = entity.tangent;
	n = entity.normal;
	b = entity.binormal;

	glPushMatrix();
	//glLoadIdentity();
/*
	gluLookAt(p.x(), p.y()+5.0, p.z()-5.0,	// camera position
		      p.x(), p.y(), p.z(),	// look at point
			  0, 0.707107f, 0.707107f);	// up vector
*/
	glTranslatef(p.x(), p.y(), p.z());

	float rMatrix[] = {t.x(), n.x(), b.x(), 0,
                       t.y(), n.y(), b.y(), 0,
                       t.z(), n.z(), b.z(), 0,
                       0, 0, 0, 1};
    glMultMatrixf(rMatrix);
	
	// for each face in the model
	for(int i = 0; i < renderObject->faceCount; i++)
	{
		// i'm assuming that all obj models will ONLY contain triangles
		// get a face

		obj_face* face = renderObject->faceList[i];

		double* Kd;
		if(renderObject->materialCount > 0)
		{
			Kd = renderObject->materialList[face->material_index]->diff;

			glColor4f((GLfloat)Kd[0], (GLfloat)Kd[1], (GLfloat)Kd[2], 1.0);
		}


		glBegin(GL_TRIANGLES);

		for(int j = 0; j < 3; j++)
		{
			obj_vector* vert = renderObject->vertexList[face->vertex_index[j]];	// get our vertex vector
			
			if(renderObject->normalCount > 0)
			{
				obj_vector* norm = renderObject->normalList[face->normal_index[j]];	// get our normal vector
                glNormal3f((GLfloat)norm->e[0], (GLfloat)norm->e[1], (GLfloat)norm->e[2]);	// set the normal
			}

			glVertex3f((GLfloat)vert->e[0], (GLfloat)vert->e[1], (GLfloat)vert->e[2]);	// draw the vertex
		}

		glEnd();
	}

	glPopMatrix();
}

void Renderer::drawPlane(float height)
{
	
	glPushMatrix();
	//glLoadIdentity();
	
	glColor4f(0.75, 0.75, 0.75, 1);
	glBegin(GL_QUADS);
	
	glNormal3f(0, 1, 0);
	glVertex3f(-100, height, -100);

	glNormal3f(0, 1, 0);
	glVertex3f(-100, height, 100);

	glNormal3f(0, 1, 0);
	glVertex3f(100, height, 100);

	glNormal3f(0, 1, 0);
	glVertex3f(100, height, -100);

	glEnd();

	glPopMatrix();
}

void Renderer::glEnable2D()
{	
	int viewport[4];
  
	glGetIntegerv(GL_VIEWPORT, viewport);
  
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
  
	glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);


}
void Renderer::glDisable2D()
{
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
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
