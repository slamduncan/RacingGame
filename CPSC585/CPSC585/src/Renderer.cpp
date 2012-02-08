#include "Renderer.h"
using namespace std;

Renderer* Renderer::instance = 0;


Renderer::Renderer()
{
	m_debugMode = 0;
	
	info = NULL;
	width = 1280;
	height = 720;
	bpp = 0;

	tm = TextureManager::initialize();
	//TextureManager::initialize();	// initialize our texture manager
}

// clean up
Renderer::~Renderer()
{
	TTF_CloseFont(debugFont);
	TTF_Quit();

	if(tm != NULL)
	{
		delete tm;
	}
	if(instance != NULL)
	{
		delete instance;
	}
}

bool Renderer::init()
{
	int a = initSDL();
	int b = initGL();
	int c =initFont();

	if(a+b+c != 0)
	{
		fprintf(stderr, "Renderer failed to init\na = %d, b = %d, c = %d\n", a, b, c);
		return false;
	}
	return true;
}

/*
*	initializes SDL window
*/
int Renderer::initSDL()
{
	int counter = 0;
	//SDL_Init( SDL_INIT_EVERYTHING );
	
	// if the video does not tempize
	if( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
	{
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        counter--;//quitSDL();
    }

	info = SDL_GetVideoInfo();

	if(!info) {
        /* This should probably never happen. */
        fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
        counter--;//quitSDL();
    }

	// 8 bits for red, green blue, 32 bit depth
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);

	vflags = SDL_OPENGL;	// set the video flags to allow opengl rendering

	bpp = info->vfmt->BitsPerPixel;	// get the bits per pixel of the screen

	// Try to set the window up
	if( SDL_SetVideoMode( width, height, bpp, vflags ) == 0 ) {
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError() );
        counter--;//quitSDL();
    }
	
	// Window created, and nothing went wrong

	return counter;
}

/*
*	NEED TO FIX, PRIVATE MEMBER VARABLES FOR W AND H
*/
int Renderer::initGL()
{
	int counter = 0;
	if(width <= 0 || height <= 0)
	{
		fprintf( stderr, "Resolution not set properly, exiting...: %s\n", SDL_GetError() );
        counter--;//quitSDL();
	}
	
	float ratio = (float) width / (float) height;	// compute FOV

    
	//
	//	GL options go here, ie glEnable, etc
	//
	
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);	// smooth shading
    glCullFace(GL_BACK);	// remove back facing surfaces
    //glFrontFace(GL_CCW);	// set front face objects to be in CCW direction
    glEnable( GL_CULL_FACE );	// allow removing culled surfaces
	glBlendFunc(GL_ONE, GL_ONE);	
	//glEnable(GL_BLEND);


	GLfloat diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat spec[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat amb[4] = {0.2f, 0.2f, 0.2f, 0.2f};
	GLfloat lightPos[4] = {5.0f, 5.0f, 0.0f, 0.0f};

	// enable lighting
	glEnable(GL_LIGHTING);
	// enable light 0
    glEnable(GL_LIGHT0);
    
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    //glMaterialf(GL_FRONT, GL_SHININESS, 1.0);
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);         // set the diffuse color for the light
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);			// set the specular color of the light
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);			// set the specular color of the light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);        // set the position of the light


	glEnable(GL_COLOR_MATERIAL);						// allow shading for colored material
	glEnable(GL_TEXTURE_2D);

	GLenum glewCheck = glewInit();	// initialize GLEW
    if (GLEW_OK != glewCheck)
    {
		std::cout << "GLEW failed to initialize" << std::endl;
		counter--;
    }
	std::cout << "GLEW initialized" << std::endl;
	
	GLenum fboCheck = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		
	// error if it isn't
	if (fboCheck != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "FBO failed to initialize" << std::endl;
		//printf("Incomplete frame buffer object\n");
		counter--;
	}
	std::cout << "FBO initialized" << std::endl;



	glClearColor( 0, 0, 0, 0 );	// clear screen to black

    glViewport( 0, 0, width, height );	// set the viewport to be the resolution of the screen

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );	// need to fix this to change fov on the fly
	glMatrixMode(GL_MODELVIEW);	// switch back to model view

	glDisable2D();

	return counter;
}

/*
*	initializes the font used.
*/
int Renderer::initFont()
{
	int counter = 0;

	if(TTF_Init())
	{
		fprintf(stderr, "Font failed to initialize: %s\n", TTF_GetError());
        //quitSDL();
		counter--;
	}

	debugFont = TTF_OpenFont("../CPSC585/font/ARIAL.TTF", 20);

	if(debugFont == NULL)
	{
		fprintf(stderr, "Font failed to load\n", 0);
        //quitSDL();
		counter--;
	}
	return counter;
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

SDL_Surface* Renderer::loadIMG(string filename)
{
	SDL_Surface* image = IMG_Load(filename.c_str());

	return image;
}
GLuint Renderer::initTexture(SDL_Surface* image)
{
	GLuint texID = 0;
	GLenum texType;

	GLint nChannel = image->format->BytesPerPixel;
	
	if (nChannel == 4)     // contains an alpha channel
    {
            if (image->format->Rmask == 0x000000ff)
                    texType = GL_RGBA;
            else
                    texType = GL_BGRA;
    } else if (nChannel == 3)     // no alpha channel
    {
            if (image->format->Rmask == 0x000000ff)
                    texType = GL_RGB;
            else
                    texType = GL_BGR;
    } else {
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

	return texID;
}
void Renderer::textureOn(GLuint texID)
{
	glBindTexture(GL_TEXTURE_2D, texID);
}
void Renderer::textureOff()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Renderer::framebufferOn(GLuint fbID)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbID);
}
void Renderer::framebufferOff()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
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
	
	vector<string> mlines;	// a vector to hold each line delimited by a '\n'
	SDL_Surface *temp;		// temporary surface to blit each line to
	SDL_Surface *toTexture;	// will contain all the text
	SDL_Color color;	// color of the text
	SDL_Rect position;	// position of the text
	
	// width and height of the texture
	int wt = 0;
	int ht = 0;
	GLuint texture;	// texture id used for binding the surface with lines
	int n = 0;	// loop variable

	// set colors
	color.r = r;
	color.g = g;
	color.b = b;

	// set the current position
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

	//glEnable(GL_TEXTURE_2D);
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

void Renderer::drawLine(btVector3 &start, btVector3 &end, int r, int g, int b, float width)
{
	assert(width >= 1);
	
	glPushMatrix();
	glLineWidth(width);

	glBegin(GL_LINES);

	glColor4f((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	
	glVertex3fv(start);
	glVertex3fv(end);
	//glVertex3f(start.getX(), start.getY(), start.getZ());
	//glVertex3f(end.getX(), end.getY(), end.getZ());

	//end.m_floats

	glEnd();

	glLineWidth(1);	// we need to reset the line width back to default as to not effect other things
	glPopMatrix();
}


/*
*	Draws a given entity to the screen
*
*	Parameters:
*	entity - the entity we want to draw
*/
void Renderer::drawEntity(Entity &entity)
{
	glPushMatrix();

	btScalar* matrix = entity.getGLMatrix();

	glMultMatrixf(matrix);

	for(int i = 0; i < (int)entity.renderObject->mNumMeshes; i++)
	{
		const aiMesh* mesh = entity.renderObject->mMeshes[i];

		/*
		if(mesh->mColors[0] != NULL) 
		{
			glEnable(GL_COLOR_MATERIAL);
		} else 
		{
			glDisable(GL_COLOR_MATERIAL);
		}*/

		if(entity.renderObject->HasMaterials())
		{

			//printf("i has found material\n");
			const aiMaterial* mat = entity.renderObject->mMaterials[mesh->mMaterialIndex];
			
			float Kd[4];
			aiColor4D diffuse;

			if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			{
				Kd[0] = diffuse.r;
				Kd[1] = diffuse.g;
				Kd[2] = diffuse.b;
				Kd[3] = diffuse.a;

				//printf("%f, %f, %f, %f\n", Kd[0], Kd[1], Kd[2], Kd[3]);

				//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
				glColor4fv(Kd);
			}
		}

		for(int j = 0; j < (int)mesh->mNumFaces; j++)
		{
			const aiFace* face = &mesh->mFaces[j];	// get a face

			GLenum face_mode;

			switch(face->mNumIndices) 
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(unsigned int k = 0; k < face->mNumIndices; k++)
			{
				int index = face->mIndices[k];
				
				/*
				// the model has a color
				// need to check this as i don't think we ever get into this if statement
				if(mesh->mColors[0] != NULL)
				{
					const aiColor4D *color = &mesh->mColors[0][index];
					
					glColor4f(color->r, color->g, color->b, color->a);
				}

				*/
				//Color4f(&mesh->mColors[0][index]);

				if(mesh->HasTextureCoords(0))
				{
					//glTexCoord2f(&mesh->mTextureCoords[index]->x, &mesh->mTextureCoords[index]->y);

					//mesh->mTextureCoords[0][index].x;


					//printf("(%f, %f)\n", mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);

					glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);

					//printf("I HAS TEXTURES\n");
				}

				// the model has normal
				if(mesh->mNormals != NULL)
				{
					glNormal3fv(&mesh->mNormals[index].x);
				}
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}


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
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-100, height, -100);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-100, height, 100);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(100, height, 100);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f,0.0f);
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

void Renderer::glEnableLighting()
{
	glEnable(GL_LIGHTING);
}
void Renderer::glDisableLighting()
{
	glDisable(GL_LIGHTING);
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



/*
*	The following functions are part of btIDebugDrawer from bullet
*/

void Renderer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
{
	glBegin(GL_LINES);
	glColor3fv(fromColor);
	glVertex3fv(from);
	glColor3fv(toColor);
	glVertex3fv(to);
	glEnd();
}

void Renderer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
	glColor3fv(color);
	glBegin(GL_LINES);

	glVertex3fv(from);
	glVertex3fv(to);

	glEnd();
}
void Renderer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
		//btVector3 to = pointOnB + normalOnB*1;	//distance;

		//const btVector3 &from = pointOnB;
		
		//glColor4fv(color);
		
		drawLine(PointOnB, PointOnB + normalOnB, color);
		
		/*
		glBegin(GL_LINES);
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
		glEnd();
		*/
}
void Renderer::reportErrorWarning(const char *warningString)
{
	printf("%s\n",warningString);
}
void Renderer::draw3dText(const btVector3 &location, const char *textString)
{
	
}
void Renderer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int Renderer::getDebugMode() const
{
	return m_debugMode;
}