#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"
#include "InputController.h"

//Test stuff
#include "EventSystemHandler.h"
#include "TestClass.h"
#include "Car.h"

// "vector" for entities
// we might just make it so that each type of specialized entity
// has their own container.
// ie. carList, powerupList, etc
#include "LinearMath\btAlignedObjectArray.h"

// Other init
// ie. Physics, AI, Renderer, Sound, Container for ents?
Renderer* ren = Renderer::getInstance();

//Test Variables
InputController controller1 = InputController();
EventSystemHandler* evSys = EventSystemHandler::getInstance();


btAlignedObjectArray<Entity*>* entityList = new btAlignedObjectArray<Entity*>();

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
		{
			// There seems to be a problem with the controller?, pressing anything besides
			// the left analog sticks causes the entity to move in the negative? xz direction
			// I also need to double check how i'm drawing things, I think i accidently made
			// opengl draw in a left handed coordinate system.
			//  Jeff
			controller1.update(event);
		
			float lX = (float)controller1.getNormalizedLeftAnalogStickX();
			float lY = (float)controller1.getNormalizedLeftAnalogStickY();

			btVector3 offset(lX, 0, lY);

			offset /= -5.0f;

			if(entityList->size() > 0)
			{
				entityList->at(0)->move(offset);
			}
		}
		break;
		case SDL_JOYBUTTONDOWN:
			fprintf(stderr, "BUTTONS HOW DO THEY WORK\n");
			controller1.update(event);
			if(controller1.isADown())
			{
				ren->quitSDL();
			}
			break;
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
	Entity *test = new Entity("../CPSC585/model/frame.obj");
	Car *car1 = new Car();
	Entity *test2 = new Entity("../CPSC585/model/frame.obj");
	Entity *test3 = new Entity("../CPSC585/model/frame.obj");

	btVector3 offset2(0.5, 0.5, 0.5);
	btVector3 offset3(-0.5, 0.5, 0.5);

	test2->position += offset2;
	test3->position += offset3;

	entityList->push_back(test);
	entityList->push_back(test2);
	entityList->push_back(test3);
	entityList->push_back(car1);

	// game loop
	while(1)
	{
		// Phyiscs


		// Inputs
		process_events();
		
		// AI
		controller1.emitTriggers();
		// Render
		// draw code goes here

		btVector3 camPos = test->position + btVector3(0, 0, -5);
		btVector3 camLookAt = test->position + btVector3(0, 0, 0);


		ren->clearGL();	// clear the screen
		//ren->setCamera(camPos, camLookAt);
		ren->setCamera(camPos, camLookAt);

		ren->drawPlane(-2);

		for(int i = 0; i < entityList->size(); i++)
		{
			ren->drawEntity(*(entityList->at(i)));
		}

		//ren->drawEntity(*test);
		//ren->drawEntity(*test2);
		//ren->drawEntity(*test3);
		//ren.draw();		// draw things to the buffer
		ren->updateGL();	// update the screen

		// Misc?
		// Compute FPS
	}

	return 0;
}