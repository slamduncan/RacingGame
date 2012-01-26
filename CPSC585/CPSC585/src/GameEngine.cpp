#include <stdio.h>
#include "Renderer.h"
#include "Entity.h"
#include "InputController.h"

//Test stuff
#include "EventSystemHandler.h"
#include "TestClass.h"
#include "Car.h"
#include <windows.h>

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
			controller1.update(event);	
		}
		break;
		case SDL_JOYBUTTONDOWN:
			fprintf(stderr, "BUTTONS HOW DO THEY WORK\n");
			controller1.update(event);

			break;
		case SDL_JOYBUTTONUP:
			controller1.update(event);
			
			break;
        }

    }

}


void updateEntityPosition(Entity &entIn, InputController &contrlIn){

			float lX = (float)contrlIn.getNormalizedLeftAnalogStickX();
			float lY = (float)contrlIn.getNormalizedLeftAnalogStickY();

			btVector3 offset(lX, 0, lY);

			offset /= 5.0f;

			entIn.move(offset);
}

void updateRot(){

			if(controller1.isADown())
			{
				ren->quitSDL();
			}
			if(controller1.isBDown())
			{
				if(entityList->size() > 0)
				{
					entityList->at(0)->rotate(entityList->at(0)->normal, -1);
				}
			}
			if(controller1.isYDown())
			{
				if(entityList->size() > 0)
				{
					entityList->at(0)->rotate(entityList->at(0)->normal, 1);
				}
			}
}

// Engine Main
int main(int argc, char** argv)
{
	// INITIALIZATIONS
	bool renInit = ren->init();
	
	//ren->initSDL();	// init SDL for drawing window
	//ren->initGL();	// initializing opengl stuff
	//ren->initFont();

	/* Added by Kent */
	controller1.initSDLJoystick();	//Init SDL joystick stuff -KD
	if (!controller1.initialize(0)){
		//SDL_Delay(100);
		/*ren->outputText("Connect Controller", 1, 0, 0, 1280/2, 720/2);
		while(!controller1.initialize(0)){
			Sleep(100);
		}*/
		/* Error on initalizing controller -KD */
	}	
		
	evSys->addObserver(&((new TestClass())->mo), EventTypes::BUTTON);

	//
	// DEBUG TESTING
	//
	Car *car1 = new Car();
	car1->loadObj("../CPSC585/model/frame.obj");
	Entity *test = new Entity("../CPSC585/model/box.obj");
	Entity *test2 = new Entity("../CPSC585/model/box.obj");
	Entity *test3 = new Entity("../CPSC585/model/box.obj");

	//Entity *planeTest = new Entity("../CPSC585/model/aaup.obj");

	//planeTest->position += btVector3(0,-1,0);

	btVector3 offset2(0.5, 0.5, 0.5);
	btVector3 offset3(-0.5, 0.5, 0.5);

	test2->position += offset2;
	test3->position += offset3;

	entityList->push_back(car1);
	entityList->push_back(test);
	entityList->push_back(test2);
	entityList->push_back(test3);

	SDL_Surface* planeTex = ren->loadIMG("../CPSC585/texture/plane.png");

	GLuint ptex = 0;

	ptex = ren->initTexture(planeTex);

	// game loop
	while(1)
	{
		// Phyiscs


		// Inputs
		process_events();
		
		// AI
		controller1.emitTriggers();
		controller1.emitButtons();
		updateRot();
		updateEntityPosition(*(entityList->at(0)), controller1);

		// Render
		// draw code goes here
		btVector3 camPos = car1->position + car1->normal*2 + car1->tangent*5;
		btVector3 camLookAt = car1->position + btVector3(0, 0, 0);
		ren->clearGL();	// clear the screen
		ren->setCamera(camPos, camLookAt);
		
		ren->textureOn(ptex);
		ren->drawPlane(-2);
		//ren->drawEntity(*planeTest);
		
		ren->textureOff();

		for(int i = 0; i < entityList->size(); i++)
		{
			ren->drawEntity(*(entityList->at(i)));
		}

		ren->glEnable2D();
		ren->outputText((*(entityList->at(0))).toString(), 0, 255, 0, 500, 200);
		ren->outputText("This is a multi\nline test to see if \nnewlines are working correctly", 255, 255, 255, 0, 360);
		ren->outputText("I am testing to see if obj models will load and draw correctly", 255, 255, 255, 0, 0);
		ren->glDisable2D();

		//ren.draw();		// draw things to the buffer
		ren->updateGL();	// update the screen

		// Misc?
		// Compute FPS
	}

	return 0;
}