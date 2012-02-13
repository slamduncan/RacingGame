#include <stdio.h>
#include "Physics.h"
#include "Renderer.h"

#include "EntityManager.h"

#include "al.h"


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
				
				int i;
				cin >> i;
			}
			if(controller1.isADown())
			{
				ren->quitSDL();
			}
			if(controller1.isXDown())
			{
				if(entManager->numCars() > 0)
				{
					//entManager->resetCarPosition(0, btVector3(0, 1.5, 0));
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


////////////////////////////////////////////////////////////////////////
// HACKED SHAPE FACTORY IN WORKS

btCollisionShape* createCarPhysicsObject()
{
	return new btBoxShape(btVector3(5, 2.5, 2.5));
}

///////////////////////////////////////////////////////////////////////////////////////////
// HACKED
btCollisionShape* createTrack(const Entity* ent)
{
	btCollisionShape* objShape;

	// Generate the physics representation
	if(ent->renderObject->HasMeshes())
	{
		// generate triangle mesh for bullet
		btTriangleMesh* bttm = new btTriangleMesh();
		
		for(unsigned int i = 0; i < ent->renderObject->mNumMeshes; i++)
		{
			aiMesh* mesh = ent->renderObject->mMeshes[i];
		
			for(unsigned int j = 0; j < mesh->mNumFaces; j++)
			{
				
				aiFace* face = &mesh->mFaces[j];	// get a face
		
				int index0 = face->mIndices[0];
				int index1 = face->mIndices[1];
				int index2 = face->mIndices[2];

				aiVector3D* v0 = &mesh->mVertices[index0];
				aiVector3D* v1 = &mesh->mVertices[index1];
				aiVector3D* v2 = &mesh->mVertices[index2];

				btVector3 btv0 = btVector3(v0->x, v0->y, v0->z);
				btVector3 btv1 = btVector3(v1->x, v1->y, v1->z);
				btVector3 btv2 = btVector3(v2->x, v2->y, v2->z);

				bttm->addTriangle(btv0, btv1, btv2, false);	// addTriangle(v0, v1, v2, removeDupes)
			}
		}

		objShape = new btBvhTriangleMeshShape(bttm, true, true);
	}

	return objShape;
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
	Car *car1 = new Car();
	Track* ground = new Track();
	Waypoint* waypoint = new Waypoint();

	btScalar carMass = 2.0;
	
	btTransform carT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 0));

	btScalar groundMass = 0.0;
	btTransform groundT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -5, 0));

	///////////////////////////////////////////////////////////////////////////////////////
	// EXTREME HACK HERE
	// I'll need to create a factory for the physics objects later so we can build almost any shape we need

	// CAR1
	car1->initRenderObject("model/box.3ds");
	btCollisionShape* carShape1 = createCarPhysicsObject();
	car1->initPhysicsObject(carShape1, carMass, carT1);
	
	// GROUND
	ground->initRenderObject("model/groundBox.lwo");
	btCollisionShape* groundShape = createTrack(ground);
	ground->initPhysicsObject(groundShape, groundMass, groundT);

	//Add the entities and physic representations
	entManager->addCar(car1);
	entManager->addTrack(ground);
	ph->addEntity(*car1);
	ph->addEntity(*ground);

	//WAYPOINT
	waypoint->initRenderObject("model/waypoint.obj");
	btCollisionShape* waypointShape = createTrack(waypoint);
	btTransform wayPointT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3.5, 0));
	btScalar temp = btScalar(0);
	waypoint->initPhysicsObject(waypointShape, temp, wayPointT); 
	entManager->addWaypoint(waypoint);	

	///* Inialize Observers used in entities */
	car1->initObservers();

	SDL_Surface* carTex1 = ren->loadIMG("model/box.png");
	GLuint ctex1 = 0;
	ctex1 = ren->initTexture(carTex1);
	SDL_FreeSurface(carTex1);

	//Set inital game time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 oldTime = SDL_GetTicks();
	int frameCount = 0;
	int counter = 1;

	//Initialize camera settings.
	btVector3 camOffset = car1->getNormal()*10 + car1->getTangent()*15;
	btVector3 camLookAt = car1->getPosition() + btVector3(0, 0, 0);
	camera1.setUpCamera(camLookAt, camOffset);

	// game loop
	while(1)
	{		
		btVector3 camLookAt = car1->getPosition() + btVector3(0, 0, 0);
	
		camera1.setUpCamera(camLookAt);
		
		//// Physics
		ph->step();

		//// Inputs
		process_events();
		
		//controller1.emitTriggers();
		//controller1.emitButtons();
		//controller1.emitLeftAnalog();
		//controller1.emitRightAnalog();

		// AI - Doesn't exist yet.....

		// Render
		ren->clearGL();	// clear the screen
		
		ren->setCamera(camera1);

		ren->glDisableLighting();
		ph->debugDraw();
		ren->glEnableLighting();

		////////////////////////////////////////////////////////
		// HACKED DRAWING need to fix this

		/* Following draws the springs for the car */
		for(int i = 0; i < entManager->numCars(); i++)
		{
			Car* temp = entManager->getCarList()->at(i);
			
			ren->textureOn(ctex1);
			ren->drawEntity(*temp);
			ren->textureOff();
			
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

		ren->outputText(entManager->getCarList()->at(0)->toString(), 0, 255, 0, 200, 200);
		ren->outputText("FPS: " + ss.str(), 0, 255, 0, 0, 700);
		
		ren->glDisable2D();


		
		////ren.draw();		// draw things to the buffer
		ren->updateGL();	// update the screen
	}

	return 0;
}