#include <stdio.h>
#include "Physics.h"
#include "Renderer.h"

#include "EntityManager.h"

//#include "Entity.h"
#include "InputController.h"
#include <time.h>
#include <sstream>
#include <iostream>

//For XML Parser
#include "ProjectNumbers.h"

//Test stuff
#include "EventSystemHandler.h"
#include "TestClass.h"
#include "Car.h"
#include "Waypoint.h"
#include <windows.h>
#include "InputMapper.h"
#include "Camera.h"

using namespace std;

// Other init
// ie. Physics, AI, Renderer, Sound, Container for ents?
Renderer* ren = Renderer::getInstance();

Physics* ph = Physics::Inst();

//Controller, camera, eventSystem handle.
InputMapper* playerInput = new InputMapper();
InputController controller1 = InputController();
Camera camera1 = Camera();
EventSystemHandler* evSys = EventSystemHandler::getInstance();

//Entity manager
EntityManager* entManager = EntityManager::getInstance();

/*
*	Handles what to do when key has been pressed
*
*/
void handle_key_down( SDL_keysym* keysym )
{
    switch( keysym->sym ) 
	{
		//Quit on esc
		case SDLK_ESCAPE:
			ren->quitSDL();
			break;
		//Reload the variables on r.
		case SDLK_r:
			evSys->emitEvent(new ReloadEvent());
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
		- updated to not l ose the event, but now must pass in controller events*/
		case SDL_JOYAXISMOTION:
		{
			controller1.update(event);	
		}
		break;
		case SDL_JOYBUTTONDOWN:
			fprintf(stderr, "BUTTONS HOW DO THEY WORK\n");
			controller1.update(event);

			if (controller1.isBDown()){
				
			}
			if(controller1.isADown())
			{
				ren->quitSDL();
			}
			if(controller1.isXDown())
			{
				if(entManager->numCars() > 0)
				{
					// resetCar(index of car, position we want to reset to)
					entManager->resetCar(0, btVector3(0, 3, 0));
				}
			}
			if(controller1.isYDown())
			{
				if(entManager->numCars() > 0)
				{
					entManager->resetCarOrientation(0);
				}
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

	//Load variables from the xml file.
	evSys->emitEvent(new ReloadEvent());	
	
	//Initialize the renderer
	bool renInit = ren->init();

	// DEBUG DRAW SETUP
	ph->setDebugDrawer(ren);
	ph->setDebugLevel(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);	// DRAW EVERYTHING
	
	/* Added by Kent */
	
	controller1.initSDLJoystick();	//Init SDL joystick stuff -KD
	
	// need to fix this so that we can maybe allow the user to know they need to plug in a controller
	if (!controller1.initialize(0)){
		/* SDL_Delay(100);
		while(!controller1.initialize(0)){
			Sleep(100);
		}*/
		/* Error on initalizing controller -KD */

		ren->quitSDL();

	}				
	
	// //RENDERER DEBUG TESTING
	
	// Create all the enitities.
	btScalar carMass = 2.0;
	
	btTransform carT1 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(0, 3, 0));	

	btTransform groundT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -5, 0));

	btTransform wayPointT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3.5, 0));
	btTransform wayPointT2 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(3.5, 3.5, 0));
	btTransform wayPointT3 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3.5, 3.5));

	entManager->createCar("model/box.3ds", carMass, carT1);	
	entManager->createTrack("model/groundBox.lwo", groundT);
	entManager->createWaypoint("model/waypoint.obj", wayPointT1);
	entManager->createWaypoint("model/waypoint.obj", wayPointT2);
	entManager->createWaypoint("model/waypoint.obj", wayPointT3);


	ren->genTexture("model/box.png", "car1");

	//Set inital game time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 oldTime = SDL_GetTicks();
	int frameCount = 0;
	int counter = 1;


	//Initialize camera settings.
	btVector3 car1N = entManager->getCar(0)->getNormal()*10;
	btVector3 car1T = entManager->getCar(0)->getTangent()*30;
	
	btVector3 camOffset = car1N + car1T;
	btVector3 camLookAt = entManager->getCar(0)->getPosition();
	camera1.setUpCamera(camLookAt, camOffset);

	// game loop
	while(1)
	{		
		camLookAt = entManager->getCar(0)->getPosition();
	
		camera1.setUpCamera(camLookAt);
		
		//// Physics
		ph->step();

		//// Inputs
		process_events();
		
		controller1.emitTriggers();
		controller1.emitButtons();
		controller1.emitLeftAnalog();
		controller1.emitRightAnalog();

		// AI - Doesn't exist yet.....

		// Render
		ren->clearGL();	// clear the screen
		
		ren->setCamera(camera1);

		ren->glDisableLighting();
		ph->debugDraw();
		ren->glEnableLighting();

		/* Following draws the springs for the car */
		for(int i = 0; i < entManager->numCars(); i++)
		{
			Car* temp = entManager->getCarList()->at(i);
			
			ren->textureOn(ren->getTexture("car1"));
			ren->drawEntity(*temp);
			ren->textureOff();
			
			// for each wheel we need to draw a line
			for(int j = 0; j < 4; j++)
			{
				//Spring aWheel = temp->wheels[j];
				Wheel aWheel = temp->newWheels[j];

				btVector3 springPos = aWheel.getAttachedToCarPosition();

				btVector3 springLength = aWheel.getBottomSpringPosition();

				ren->drawLine(springPos, springLength, 0, 0, 255, 3.0f);

			}
		}
		
		//Draw the waypoints.
		for(int i = 0; i < entManager->numWaypoints(); i++)
		{
			ren->drawEntity(*(entManager->getWaypointList()->at(i)));
		}

		// Draw the track.
		if(entManager->getTrack())
		{
			ren->drawEntity(*(entManager->getTrack()));
		}
		
		ren->glEnable2D();
		frameCount++;

		/* Calculate the frames per second */
		if((currentTime - oldTime) > 1000){
			//sprintf_s(frames, "%d FPS", frameCount);	
			//ren->outputText(frames, 0, 255, 0, 10, 700);
			//std::cout << frameCount << "\n";
			//frameCount = 0;
			counter++;
			oldTime = currentTime;
		}		
		currentTime = SDL_GetTicks();
		
		std::stringstream ss;
		ss << frameCount/counter;

		ren->outputText(entManager->getCarList()->at(0)->toString(), 255, 255, 255, 200, 200);
		ren->outputText("FPS: " + ss.str(), 0, 255, 0, 0, 700);
		
		ren->glDisable2D();

		ren->updateGL();	// update the screen
	}

	return 0;
}