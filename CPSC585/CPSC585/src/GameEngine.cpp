#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"
#include "InputController.h"

//Test stuff
#include "EventSystemHandler.h"
#include "TestClass.h"

#include "LinearMath\btAlignedObjectArray.h"

// Other init
// ie. Physics, AI, Renderer, Container for ents?
Renderer* ren = Renderer::getInstance();

//Test Variables
InputController controller1 = InputController();
EventSystemHandler* evSys = EventSystemHandler::getInstance();


btAlignedObjectArray<Entity> entityList;

/*
*	Handles what to do when key has been pressed
*
*/
void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym ) 
	{
		case SDLK_ESCAPE:
			ren->quitSDL();
			break;
		default:
			break;
    }

}

/*
*	Handles SDL events that are sent by user
*/
void process_events()
{
    /* Our SDL event placeholder. */
    SDL_Event event;
	
    /* Grab all the events off the queue. */
    while(SDL_PollEvent( &event )) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            // QUIT SDL
			ren->quitSDL();
            break;
		/* Handle controller Events ?  Does this lose the event?
		- updated to not lose the event, but now must pass in controller events*/
		case SDL_JOYAXISMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			controller1.update(event);
			break;
        }

    }

}


// Engine Main
int main(int argc, char** argv)
{
	// INITIALIZATIONS
	ren->initSDL();	// init SDL for drawing window
	ren->initGL(1280, 720);	// initializing opengl stuff
	ren->initFont();

	/* Added by Kent */
	controller1.initSDLJoystick();	//Init SDL joystick stuff -KD
	if (!controller1.initialize(0)){
		/* Error on initalizing controller -KD */
	}	

	//
	// DEBUG TESTING
	//
	Entity test("../CPSC585/model/frame.obj");

	// game loop
	while(1)
	{
		// Phyiscs


		// Inputs
		process_events();

		// AI


	

		
		// Render
		// draw code goes here

		btVector3 lookAtPoint = test.position + btVector3(0, 5, -5);

		ren->clearGL();	// clear the screen
		ren->drawPlane(-2);
		//ren->setCamera(test.position, lookAtPoint);
		ren->drawEntity(test);
		//ren.draw();		// draw things to the buffer
		ren->updateGL();	// update the screen

		// Misc?
		// Compute FPS
	}

	return 0;
}