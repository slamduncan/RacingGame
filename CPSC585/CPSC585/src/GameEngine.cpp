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
#include "AIHandler.h"

#include "Sound.h"

using namespace std;

// Other init
// ie. Physics, AI, Renderer, Sound, Container for ents?
Renderer* ren = Renderer::getInstance();
AIHandler* ai = AIHandler::getInstance();
Physics* ph = Physics::Inst();

//Controller, camera, eventSystem handle.
InputMapper* playerInput = new InputMapper();
InputController controller1 = InputController();
Camera camera1 = Camera();
EventSystemHandler* evSys = EventSystemHandler::getInstance();

//Entity manager
EntityManager* entManager = EntityManager::getInstance();

// TESTING AREA
bool depthShader = false;

//Dynamic creation of waypoints
void createWaypoint(){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	Car* c = entManager->getCar(0);
	//btScalar a = c->getTangent().angle(btVector3(1,0,0));
//	btTransform wayPointT1 = btTransform(btQuaternion(btVector3(0,1,0),a),entManager->getCar(0)->getPosition() + btVector3(0,3,0));
	btTransform wayPointT1 = c->physicsObject->getWorldTransform();
	//btTransform wayPointT1 = btTransform(btQuaternion(0, 0, 0, 1),entManager->getCar(0)->getPosition() + btVector3(0,3,0));
	Waypoint* previousWay;
	if (wayList->size()>0)
	{
		previousWay = wayList->at(wayList->size()-1);
		previousWay->removeWaypointFromList(wayList->at(0)->getIndex());
	}
	entManager->createWaypoint("model/waypoint.obj", wayPointT1);
	
	Waypoint* newWay = wayList->at(wayList->size()-1);
	if (wayList->size()>1)
	{
		previousWay->addNextWaypoint(newWay);
		newWay->addNextWaypoint(wayList->at(0));
	}
	newWay->setThrottle(controller1.getTriggers());
}

int getClosestWaypoint(Car* car = entManager->getCar(0)){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	//Car* car = entManager->getCar(0);
	float distance = 10000.0;
	int index = -1;
	Waypoint* currentWaypoint;
	for (int i = 0; i < wayList->size(); i++)
	{
		currentWaypoint = wayList->at(i);
		float tempDist = (car->getPosition() - currentWaypoint->getPosition()).length();
		if (tempDist < distance){
			distance = tempDist;
			index = i;
		}
	}
	return index;
}

/* NOTE: THIS FUNCTION IS ONLY DEFINED FOR WHEN THERE IS AN ORDERED LIST OF SINGLE WAYPOINTS */
//Issues exist with this funtion...
void deleteWaypoint(){
	int index = getClosestWaypoint();
	if (index != -1)
	{
		btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
		if (index != 0)
		{
			wayList->at(index-1)->removeWaypointFromList(index);
			wayList->at(index-1)->addNextWaypoint(wayList->at(index + 1));
		}
		else{
			wayList->at(wayList->size()-1)->removeWaypointFromList(index);
			wayList->at(wayList->size()-1)->addNextWaypoint(wayList->at(index + 1));
		}

		wayList->at(index)->initRenderObject("model/deletedWaypoint.obj");
	}
}

void writeWaypoints(const char* fileName){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	ofstream file(fileName);
	if (file.is_open())
	{
		for (int i = 0; i < wayList->size(); i++)
		{
			std::string temp = wayList->at(i)->toString();
			file << temp;
		}
		file.close();
	}
	else
		printf("Unable to open Waypoint File - Write\n");
}

void moveWaypoint(){
	int index = getClosestWaypoint();
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	Waypoint* w = wayList->at(index);
	Car* car = entManager->getCar(0);	

	btVector3 tan = w->getWaypointList().at(0)->getPosition() - w->getPosition();
	tan.normalize();
	btVector3 biNorm = tan.cross(btVector3(0,1,0));
	btVector3 normal = biNorm.cross(tan);

	btMatrix3x3 matrix = btMatrix3x3(tan.x(), normal.x(), biNorm.x(), tan.y(), normal.y(), biNorm.y(), tan.z(), normal.z(), biNorm.z());
	btTransform cT = btTransform(matrix, car->getPosition());	
	w->setTransform(cT);
}

void readWaypoints(const char* fileName){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	wayList->clear();
	ifstream file(fileName);
	string line;
	stringstream ss;
	if (file.is_open())
	{
		while (file.good()){
			getline(file, line);
			ss << line;
			float x, y, z, r1, r2, r3, r4,r5,r6, r7,r8,r9;
			int throttle;
			ss >> x;
			ss >> y;
			ss >> z;
			ss >> r1;
			ss >> r2;
			ss >> r3;

			ss >> r4;
			ss >> r5;
			ss >> r6;

			ss >> r7;
			ss >> r8;
			ss >> r9;

			ss >> throttle;
			btMatrix3x3 temp = btMatrix3x3(r1,r2,r3,r4,r5,r6,r7,r8,r9);
			btTransform wayPointT1 = btTransform(temp, btVector3(x, y, z));
			entManager->createWaypoint("model/waypoint.obj", wayPointT1, throttle);
		}
		for (int i = 0; i < wayList->size()-1; i++)
		{
			Waypoint* w1 = wayList->at(i);
			Waypoint* w2 = wayList->at(i+1);
			w1->addNextWaypoint(w2);
			stringstream ss;
			ss << w1->getIndex();
			ren->draw3dText(w1->getPosition(), ss.str().c_str());
		}
		
		Waypoint* w1 = wayList->at(0);
		Waypoint* w2 = wayList->at(wayList->size()-1);
		w2->addNextWaypoint(w1);

		for (int i = 0; i <= wayList->size()-1; i++){
			btVector3 tan = wayList->at(i)->getWaypointList().at(0)->getPosition() - wayList->at(i)->getPosition();
			tan.normalize();
			btVector3 biNorm = tan.cross(btVector3(0,1,0));
			btVector3 normal = biNorm.cross(tan);

			btMatrix3x3 matrix = btMatrix3x3(tan.x(), normal.x(), biNorm.x(), tan.y(), normal.y(), biNorm.y(), tan.z(), normal.z(), biNorm.z());
			btTransform t = btTransform(matrix, wayList->at(i)->getPosition());
			wayList->at(i)->setTransform(t);
		}
		
		file.close();
		for (int i = 1; i < entManager->getCarList()->size(); i++)
			entManager->getCar(i)->setNextWaypointIndex(0);
		//Update the waypoint variables.
		evSys->emitEvent(new ReloadEvent());
	}
	else
		printf("Unable to open Waypoint File - Read\n");
}


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
			{
				ren->quitSDL();
				break;
			}
		//Reload the variables on r.
		case SDLK_r:
			{
				evSys->emitEvent(new ReloadEvent());
				break;
			}
		case SDLK_w:
			{
				writeWaypoints("waypoints.w");
				break;
			}
		case SDLK_l:
			{
				//readWaypoints("waypoints.w");
				readWaypoints("sandboxWaypoints.w");
				break;
			}
		default:
			{
				break;
			}
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

			if (controller1.isBDown())
			{
				createWaypoint();
			}
			if(controller1.isADown())
			{				
				//deleteWaypoint();
				//int i;
				//cin >> i;
				moveWaypoint();
			}
			if(controller1.isXDown())
			{
				if(entManager->numCars() > 0)
				{
					// resetCar(index of car, position we want to reset to)
					int index = getClosestWaypoint();
					if (entManager->getWaypointList()->size() > 0 && index != -1)
					{
						Waypoint *w = entManager->getWaypoint(index);						
						btTransform trans = w->getTransform();
						btQuaternion q = btQuaternion(btVector3(0,1,0), trans.getRotation().getAngle() + SIMD_PI);
						trans.setRotation(q);
						entManager->resetCar(0, trans);
					}
					else
					{
						entManager->resetCar(0, btVector3(0, 3, 0));
					}

				}
			}
			if(controller1.isYDown())
			{
				if(entManager->numCars() > 0)
				{
					entManager->resetCarOrientation(0);
				}
			}
			if(controller1.isButtonDown(controller1.R_Bump))
			{
				if(depthShader)
				{
					depthShader = false;
				}
				else
				{
					depthShader = true;
				}
			}
			if (controller1.isButtonDown(controller1.Start_button))
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

	
	//Initialize the renderer
	bool renInit = ren->init();

	// DEBUG DRAW SETUP
	ph->setDebugDrawer(ren);
	//ph->setDebugLevel(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);	// DRAW EVERYTHING
	ph->setDebugLevel(btIDebugDraw::DBG_NoDebug);	// DRAW EVERYTHING

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
	btTransform carT2 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(15, 3, 0));	

	btTransform groundT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -5, 0));

	btTransform wayPointT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3.5, -2));
	btTransform wayPointT2 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(25, 3.5, 0));
	btTransform wayPointT3 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3.5, 3.5));
	
	btTransform powerupT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -10));


	entManager->createCar("model/box.3ds", carMass, carT1);	
	/*for(int i = 0; i < 30; i++){
		btTransform carT2 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(15*i, 3, 0));	
		entManager->createCar("model/box.3ds", carMass, carT2);	
	}*/

	entManager->createCar("model/box.3ds", carMass, carT2);	
	
	entManager->createTrack("model/groundBox.lwo", groundT);
	//entManager->createTrack("model/Track1tri.lwo", groundT);
	
	//entManager->createWaypoint("model/waypoint.obj", wayPointT1);
	//entManager->createWaypoint("model/waypoint.obj", wayPointT2);
	//btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	//for (int i = 0; i < wayList->size()-1; i++)
	//{
	//	Waypoint* w1 = wayList->at(i);
	//	Waypoint* w2 = wayList->at(i+1);
	//	w1->addNextWaypoint(w2);
	//}
	//Waypoint* w1 = wayList->at(0);
	//Waypoint* w2 = wayList->at(wayList->size()-1);
	//w2->addNextWaypoint(w1);

	//entManager->createWaypoint("model/waypoint.obj", wayPointT3);
	entManager->createPowerup("model/powerup.lwo", powerupT1);

	//Set inital game time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 oldTime = SDL_GetTicks();
	int frameCount = 0;
	int counter = 1;
	int instantFrameCount = 0;
	stringstream instantFrameCountBuffer;
	string instantFrameString = "";


	//Initialize camera settings.
	btVector3 car1N = entManager->getCar(0)->getNormal()*10;
	btVector3 car1T = entManager->getCar(0)->getTangent()*30;
	
	btVector3 camOffset = car1N + car1T;
	btVector3 camLookAt = entManager->getCar(0)->getPosition();
	camera1.setUpCamera(camLookAt, camOffset);

	//LoadBackgroundSoundFile("Documentation/Music/Engine.wav");
	LoadBackgroundSoundFile("Documentation/Music/In Game Music.wav");

	//Load variables from the xml file.
	evSys->emitEvent(new ReloadEvent());	

	// game loop
	while(1)
	{		
		//alSourcef(source, AL_PITCH, 1.0f + entManager->getCar(0)->GetSpeed() );

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
		ai->generateNextMove();

		// Render
		ren->clearGL();	// clear the screen

		ren->glDisableLighting();
		ph->debugDraw();
		ren->glEnableLighting();
/*
		if(depthShader)
		{
			ren->draw(ssao1);
		}
		else
		{
			ren->drawAll();
		}
*/
		//ren->draw(test);

		ren->shadowMapPass();

		ren->setCamera(camera1);
		//ren->drawTexture("depth2l1");

		ren->drawAll();
/*
		//Following draws the springs for the car
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
*/		

		ren->glEnable2D();
		frameCount++;
		instantFrameCount++;

		/* Calculate the frames per second */
		if((currentTime - oldTime) > 1000){
			//sprintf_s(frames, "%d FPS", frameCount);				
			std::stringstream ssInstant;
			ssInstant << instantFrameCount << " Instant FPS";
			//ren->outputText(frames, 0, 255, 0, 10, 700);
			//std::cout << frameCount << "\n";
			//frameCount = 0;
			instantFrameCount = 0;
			instantFrameString = ssInstant.str();			
			counter++;
			oldTime = currentTime;			
		}		
		currentTime = SDL_GetTicks();
		
		std::stringstream ss;
		ss << frameCount/counter;

		ren->outputText(entManager->getCarList()->at(0)->toString(), 255, 255, 255, 200, 200);
		ren->outputText("FPS: " + ss.str(), 0, 255, 0, 0, 700);
		ren->outputText("FPS: " + instantFrameString, 0, 255, 0, 0, 680);
		
		ren->glDisable2D();

		ren->updateGL();	// update the screen
	}

	return 0;
}