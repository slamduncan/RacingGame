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
#include "Timer.h"

#include "Sound.h"
#define SANDBOX 0
using namespace std;

// FPS LIMITING DATA
const int TICKS_PER_SECONDS = 60;
const int SKIP_TICKS = 1000/TICKS_PER_SECONDS;
const int MAX_FRAMESKIP = 5;

ALuint EngineSource = 2;


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
int waypointIndex1 = 0, waypointIndex2 = 0;

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
			index = wayList->at(i)->getIndex();
		}
	}
	return index;
}


void addWaypointInbetween()
{
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	int prevIndex = getClosestWaypoint();
	if (prevIndex == -1)
		return;
	/*else if (prevIndex == wayList->size()-1)
	{
		createWaypoint();
		return;
	}*/
	Car* c = entManager->getCar(0);
	

	btTransform wayPointT1 = c->physicsObject->getWorldTransform();
	
	Waypoint* previousWay = wayList->at(prevIndex);		
	int nextIndex = previousWay->getWaypointList().at(0)->getIndex();
	previousWay->removeWaypointFromList(wayList->at(nextIndex)->getIndex());
	
	entManager->createWaypoint("model/waypoint.obj", wayPointT1);
	
	Waypoint* newWay = wayList->at(wayList->size()-1);
	if (wayList->size()>1)
	{
		previousWay->addNextWaypoint(newWay);
		newWay->addNextWaypoint(wayList->at(nextIndex));
	}
	newWay->setThrottle(controller1.getTriggers());
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
		Waypoint* nextWaypoint = wayList->at(0);
		for (int i = 0; i < wayList->size(); i++)
		{
			std::string temp = nextWaypoint->toString();
			file << temp;
			if (nextWaypoint->getWaypointList().size() > 0)
			/*	if (nextWaypoint->split)
				{
					std::string temp = nextWaypoint->toString();
					file << temp;
					Waypoint* partOfPath = nextWaypoint;
					for(int count = 0; count < nextWaypoint->getWaypointList().size();;)
					{
						partOfPath = partOfPath->getWaypointList(0);
						if (
					}
				}
				else*/
					nextWaypoint = nextWaypoint->getWaypointList().at(0);
			else
				break;			
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

void markWaypointSplit(){
	int index = getClosestWaypoint();
	entManager->getWaypoint(index)->split = !entManager->getWaypoint(index)->split;
}

void saveWaypointIndex1(){
	waypointIndex1 = getClosestWaypoint();
}

void savedWaypointIndex2(){
	waypointIndex2 = getClosestWaypoint();
}

void markConverge(){
	entManager->getWaypoint(getClosestWaypoint())->converge = true;
}

void connectWaypointsTruncate(){
	Waypoint* w1 = entManager->getWaypoint(waypointIndex1);
	Waypoint* w2 = entManager->getWaypoint(waypointIndex2);
	w1->getWaypointList().clear();
	w1->addNextWaypoint(w2);
}

void connectWaypointsAddition(){
	Waypoint* w1 = entManager->getWaypoint(waypointIndex1);
	Waypoint* w2 = entManager->getWaypoint(waypointIndex2);
	w1->addNextWaypoint(w2);
}


void readWaypoints(const char* fileName){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	wayList->clear();
	ifstream file(fileName);
	string line;
	stringstream ss;
	bool split = false;
	btAlignedObjectArray<Waypoint*> convergeList, splitList;
	if (file.is_open())
	{
		while (file.good()){
			getline(file, line);
			if (line.compare("SPLIT") == 0)
			{
				getline(file, line);
				split = true;
//				splitList.push_back(entManager->getWaypoint(entManager->getWaypointList()->size()-1);
				ss << line;				
				int count;
				ss >> count;

				Waypoint* lastWaypoint;
				Waypoint* splitWaypoint = entManager->getWaypoint(entManager->getWaypointList()->size()-1);				
				bool addSplitWaypoint = true;
				int splitCount = 0;
				
				while(splitCount != count)
				{
					getline(file, line);
					if (line.compare("CONVERGE") == 0)
					{						
						addSplitWaypoint = true;
						convergeList.push_back(lastWaypoint);
						splitCount++;
						continue;					
					}
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
					lastWaypoint = entManager->getWaypoint(entManager->getWaypointList()->size()-1);
					if (addSplitWaypoint)
					{
						splitWaypoint->addNextWaypoint(lastWaypoint);
						addSplitWaypoint = false;
					}
				}				
			}
			if (split)
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
			if (split)
			{
				split = false;
				Waypoint* convergeWaypoint = entManager->getWaypoint(entManager->getWaypointList()->size()-1);
				for (int i = 0; i < convergeList.size(); i++)
				{
					convergeList.at(i)->addNextWaypoint(convergeWaypoint);
				}

			}
		}
		for (int i = 0; i < wayList->size()-1; i++)
		{
			Waypoint* w1 = wayList->at(i);
			Waypoint* w2 = wayList->at(i+1);
			if (w1->getWaypointList().empty())
				w1->addNextWaypoint(w2);
			/*stringstream ss;
			ss << w1->getIndex();
			ren->draw3dText(w1->getPosition(), ss.str().c_str());*/
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
		for (int i = 0; i < entManager->getCarList()->size(); i++)
			entManager->getCar(i)->setNextWaypointIndex(0);
		//Update the waypoint variables.
		ReloadEvent* e = new ReloadEvent();
		evSys->emitEvent(e);
		delete e;
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
				ReloadEvent* e = new ReloadEvent();
				evSys->emitEvent(e);
				delete e;
				break;
			}
		case SDLK_w:
			{
#if SANDBOX
				writeWaypoints("sandboxWaypoints.w");
#else
				//writeWaypoints("waypoints.w");
#endif
				break;
			}
		case SDLK_l:
			{
#if SANDBOX
				readWaypoints("sandboxWaypoints.w");				
#else
				readWaypoints("waypoints.w");
#endif
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
			//fprintf(stderr, "BUTTONS HOW DO THEY WORK\n");
			controller1.update(event);

			if (controller1.isBDown())
			{
				//createWaypoint();

			}
			if(controller1.isADown())
			{				
				//deleteWaypoint();
				//int i;
				//cin >> i;
				//moveWaypoint();

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
				int index = getClosestWaypoint();
				if (entManager->getWaypointList()->size() > 0 && index != -1)
				{
					Waypoint *w = entManager->getWaypoint(index);
					btTransform trans = w->getTransform();
					
					btMatrix3x3 basis = trans.getBasis();

					btTransform cTrans = entManager->getCar(0)->physicsObject->getWorldTransform();
					cTrans.setBasis(basis);
					entManager->getCar(0)->physicsObject->setWorldTransform(cTrans);

				}
				else
				{
					entManager->resetCarOrientation(0);
				}

			}
			if(controller1.isButtonDown(controller1.R_Bump))
			{

			}
			if(controller1.isButtonDown(controller1.L_Bump))
			{

				//printf("Trying to use a speed boost...\n");
				entManager->getCar(0)->UsePowerUp(0);

			}

			if (controller1.isButtonDown(controller1.Start_button))
			{
				readWaypoints("waypoints.w");

				LoadSoundFile("Documentation/Music/Engine.wav", &EngineSource);
				LoadBackgroundSoundFile("Documentation/Music/InGameMusic.wav");

			}
			if (controller1.isButtonDown(controller1.Back_button))
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


void resetCars(){
	for (int i = 0; i < entManager->getCarList()->size(); i ++)
	{
		Car* c = entManager->getCar(i);
		if (c->getPosition().y() < -300.0)
		{
			// resetCar(index of car, position we want to reset to)
			int index = getClosestWaypoint(c);
			if (entManager->getWaypointList()->size() > 0 && index != -1)
			{
				Waypoint *w = entManager->getWaypoint(index);						
				btTransform trans = w->getTransform();
				btQuaternion q = btQuaternion(btVector3(0,1,0), trans.getRotation().getAngle() + SIMD_PI);
				trans.setRotation(q);
				entManager->resetCar(i, trans);
			}
			else
			{
				entManager->resetCar(i, btVector3(0, 3, 0));
			}
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

	btTransform wayPointT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.f, 3.5f, -2.f));
	btTransform wayPointT2 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(25.f, 3.5f, 0.f));
	btTransform wayPointT3 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.f, 3.5f, 3.5f));
	
	for(int i = 1; i < 5; i++){
		btTransform powerupT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.f, 7.5f, 50.f*i));
		entManager->createPowerup("model/powerup.lwo", powerupT1);
	}

	entManager->createCar("model/ship1.lwo", carMass, carT1);	
	entManager->createCar("model/ship1.lwo", carMass, carT2);	
	for(int i = 1; i < 2; i++){
		btTransform carT3 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(0.0f, 3.0f, (float)i*-30.0f));	
		entManager->createCar("model/ship1.lwo", carMass, carT3);	
		btTransform carT4 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(30.0f, 3.0f, (float)i*-30.0f));	
		entManager->createCar("model/ship1.lwo", carMass, carT4);	
	}
	
	
#if SANDBOX
	entManager->createTrack("model/groundBoxTri.lwo", groundT);
#else
	entManager->createTrack("model/Track1tri.lwo", groundT);
#endif

	// Variables for lap time
	int LapMinutes = 0;
	int LapSeconds = 0;
	int LapMilliseconds = 0;

	//Initialize camera settings.
	btVector3 car1N = entManager->getCar(0)->getNormal()*10;
	btVector3 car1T = entManager->getCar(0)->getTangent()*30;
	
	btVector3 camOffset = car1N + car1T;
	btVector3 camLookAt = entManager->getCar(0)->getPosition();
	camera1.setUpCamera(camLookAt, camOffset);

	//LoadSoundFile("Documentation/Music/Engine.wav", &EngineSource);
	//LoadBackgroundSoundFile("Documentation/Music/InGameMusic.wav");

	//Load variables from the xml file.
	ReloadEvent* e = new ReloadEvent();
	evSys->emitEvent(e);
	delete e;

	float EngineModifier = 0;

	int LapNumber = 1;
	int WaypointIndex = -1;
	int CurrentWaypointIndex = 0;

	//Set inital game time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 oldTime = SDL_GetTicks();
	int frameCount = 0;
	int counter = 1;
	int instantFrameCount = 0;
	stringstream instantFrameCountBuffer;
	string instantFrameString = "";

	int loops;
	float interpolation;
	bool running = true;

	Uint32 next_game_tick = SDL_GetTicks();
	// game loop
	while(running)
	{		
/*
			process_events();
			//controller1.emitTriggers();
			//controller1.emitButtons();
			//controller1.emitLeftAnalog();
			//controller1.emitRightAnalog();
			
			ren->clearGL();

			ren->glEnable2D();
			ren->changeFontSize(100);
			ren->outputText("Varios", 255, 255, 255, 500, 500);

			ren->changeFontSize(20);
			ren->outputText("Press start", 255, 255, 255, 600, 200);

			ren->glDisable2D();
			ren->updateGL();
*/
		loops = 0;
			
		while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP)
		{
			// Calculate engine's change in pitch
			EngineModifier = (entManager->getCar(0)->GetSpeed() / (-1 * entManager->getCar(0)->GetForwardForceModifier()));

			// If going in reverse, we still want engine to rev up
			if(EngineModifier < 0)
				EngineModifier *= -1;

			// Change pitch of engine sound
			alSourcef(EngineSource, AL_PITCH, 1.0f + EngineModifier );
			
			//// Physics
			ph->step();

			//// Inputs
			process_events();
			
			controller1.emitTriggers();
			controller1.emitButtons();
			controller1.emitLeftAnalog();
			controller1.emitRightAnalog();

			// AI
			ai->generateNextMove();
			

			// Calculate current lap for player's car
			WaypointIndex = entManager->getCar(0)->getNextWaypointIndex();
			if( WaypointIndex != CurrentWaypointIndex )
			{
				CurrentWaypointIndex = WaypointIndex;
				if( CurrentWaypointIndex == 0 )
				{
					LapNumber++;
					LapMinutes = 0;
					LapSeconds = 0;
					LapMilliseconds = 0;
				}
			}

			next_game_tick += SKIP_TICKS;
			loops++;
			instantFrameCount++;
		}

		interpolation = float(SDL_GetTicks() + SKIP_TICKS - next_game_tick/ float(SKIP_TICKS));

		// Render
		ren->clearGL();	// clear the screen
/*
		ren->glDisableLighting();
		ph->debugDraw();
		ren->glEnableLighting();
*/
		ren->shadowMapPass();
		
		ren->clearGL();

		//ren->drawTexture("depth2l1");
		
		camLookAt = entManager->getCar(0)->getPosition();
		camera1.setUpCamera(camLookAt);
		//camera1.updateCamera(camLookAt, entManager->getCar(0)->getBinormal());
		
		ren->setCamera(camera1);
		//ren->draw();

		//ren->clearGL();		
		//ren->setCamera(camera1);
		ren->drawAll();

		ren->glEnable2D();
	
		ren->changeFontSize(20);
		ren->outputText(entManager->getCarList()->at(0)->toString(), 255, 255, 255, 200, 200);

		int TimeDifference = currentTime - oldTime;
		if((TimeDifference) > 1000){
			std::stringstream ssInstant;
			//sprintf_s(frames, "%d FPS", frameCount);				
			ssInstant << instantFrameCount << " Instant FPS";
			//ren->outputText(frames, 0, 255, 0, 10, 700);
			//std::cout << frameCount << "\n";
			//frameCount = 0;
			instantFrameCount = 0;
			instantFrameString = ssInstant.str();			
			counter++;
			oldTime = currentTime;		
			
			LapSeconds++;
		}		
		currentTime = SDL_GetTicks();

		ren->outputText("FPS: " + instantFrameString, 0, 255, 0, 0, 680);
/*
		frameCount++;
		instantFrameCount++;

		int TimeDifference = currentTime - oldTime;

		// Calculate the frames per second 
		if((TimeDifference) > 1000){
			std::stringstream ssInstant;
			//sprintf_s(frames, "%d FPS", frameCount);				
			ssInstant << instantFrameCount << " Instant FPS";
			//ren->outputText(frames, 0, 255, 0, 10, 700);
			//std::cout << frameCount << "\n";
			//frameCount = 0;
			instantFrameCount = 0;
			instantFrameString = ssInstant.str();			
			counter++;
			oldTime = currentTime;		
			LapSeconds++;
		}		
		currentTime = SDL_GetTicks();
		
		std::stringstream ss;
		ss << frameCount/counter;

		
		ren->outputText("FPS: " + ss.str(), 0, 255, 0, 0, 700);
		ren->outputText("FPS: " + instantFrameString, 0, 255, 0, 0, 680);

		//printf("NP: %d\n", entManager->getCar(0)->GetNumberPowerUps());
		//entManager->getCarList()->at(0)->outputPowerups();
*/

		
		std::stringstream ssLapTime;
		std::stringstream ssLap;
		ssLap << LapNumber;

		// Calculate the current lap time
		if( LapMinutes < 10 )
			ssLapTime << "0";
		ssLapTime << LapMinutes << ":";

		if( LapSeconds >= 60 )
		{
			LapSeconds = 0;
			LapMinutes++;
		}
		else if( LapSeconds < 10 )
			ssLapTime << "0";
		ssLapTime << LapSeconds << ":";

		if( TimeDifference < 100 )
			ssLapTime << "0";
		else if( TimeDifference >= 1000 )
			TimeDifference = 999;
		
		LapMilliseconds = TimeDifference / 10;
		ssLapTime << LapMilliseconds;

		// Display the current lap time
		ren->outputText("Current Lap: " + ssLapTime.str(), 255, 0, 0, 0, 660);
		ren->outputText("Lap: " + ssLap.str(), 255, 0, 0, 0, 640);
		
		ren->glDisable2D();

		ren->updateGL();	// update the screen

		resetCars();
		
	}

	return 0;
}