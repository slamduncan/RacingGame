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
#include "Rocket.h"
#include "Menu.h"
#include "Math.h"

#include "SoundPlayer.h"
#define SANDBOX 0
#define EDIT_WAYPOINTS 0
using namespace std;

// FPS LIMITING DATA
const int TICKS_PER_SECONDS = 60;
const int SKIP_TICKS = 1000/TICKS_PER_SECONDS;
const int MAX_FRAMESKIP = 5;

bool stillWantsToPlay = true;

ALuint EngineSource = 1;
ALuint EngineBuffer = 2;

ALuint BackgroundSource = 3;
ALuint BackgroundBuffer = 4;

ALuint MenuSource = 5;
ALuint MenuBuffer = 6;

enum GameState {MAIN_MENU, LOADING_GAME, GAME_STARTING, GAME_RUNNING, GAME_FINISHED, PAUSED_IN_GAME};
GameState CURRENT_STATE = MAIN_MENU;
// Other init
// ie. Physics, AI, Renderer, Sound, Container for ents?
Renderer* ren = Renderer::getInstance();
AIHandler* ai = AIHandler::getInstance();
Physics* ph = Physics::Inst();


SoundPlayer soundPlayer;

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

void floatingWaypoint(){
	btAlignedObjectArray<Waypoint*>* wayList = entManager->getWaypointList();
	Car* c = entManager->getCar(0);
	btTransform wayPointT1 = c->physicsObject->getWorldTransform();	
	entManager->createWaypoint("model/waypoint.obj", wayPointT1);	
	Waypoint* newWay = wayList->at(wayList->size()-1);
	newWay->setThrottle(controller1.getTriggers());
	Waypoint* prevWay = wayList->at(waypointIndex1);
	prevWay->addNextWaypoint(newWay);
	waypointIndex1 = newWay->getIndex();
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
		for (int i = 0; i < wayList->size();)
		{
			std::string temp = nextWaypoint->toString();
			file << temp;
			i++; //Wrote One Waypoint
			if (nextWaypoint->getWaypointList().size() > 0)
				if (nextWaypoint->split)
				{
					//std::string temp = nextWaypoint->toString();
					//file << temp;
					Waypoint* partOfPath = nextWaypoint;
					for(unsigned int count = 0; count < nextWaypoint->getWaypointList().size(); count++)
					{
						/*Note: issue here if you have multi-path in multi-path */
						partOfPath = nextWaypoint->getWaypointList().at(count);
						while(!partOfPath->converge)
						{
							temp = partOfPath->toString();
							file << temp;
							i++; //Wrote one waypoint
							partOfPath = partOfPath->getWaypointList().at(0);
						}
						temp = partOfPath->toString();
						file << temp;
						i++; //Wrote one waypoint
					}
					nextWaypoint = partOfPath->getWaypointList().at(0);
				}
				else
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
	//entManager->getWaypoint(getClosestWaypoint())->converge = true;
	entManager->getWaypoint(waypointIndex1)->converge = true;
	entManager->getWaypoint(waypointIndex1)->getWaypointList().clear();
	entManager->getWaypoint(waypointIndex1)->addNextWaypoint(entManager->getWaypoint(waypointIndex2));

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
			if(line.compare("\n") == 0 || line.empty())
				continue;
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
						lastWaypoint->converge = true;
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
				convergeList.clear();
				splitList.clear();
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
		ReloadEvent e = ReloadEvent();
		e.read();
		evSys->emitEvent(&e);
		//delete e;
	}
	else
		printf("Unable to open Waypoint File - Read\n");
}

void loadPowerupLocation(char* fname)
{
	std::ifstream file(fname);

	// if the file does exist.
	if(file)
	{
		std::ofstream pfile;
		pfile.open("poweruploc.p");
		
		const aiScene* ploc = aiImportFile(fname, 
			aiProcess_CalcTangentSpace       |
			//aiProcess_Triangulate            |
			//aiProcess_JoinIdenticalVertices  |
			aiProcess_GenSmoothNormals |
			aiProcess_ImproveCacheLocality |
			aiProcess_GenUVCoords |
			aiProcess_TransformUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_SortByPType |
			aiProcess_FlipUVs);

		for(unsigned int i = 0; i < ploc->mNumMeshes; i++)
		{
			const aiMesh* mesh = ploc->mMeshes[i];

			for(unsigned int vert = 0; vert < mesh->mNumVertices; vert++)
			{
				aiVector3D vertex = mesh->mVertices[vert];
				pfile << vertex.x << " " << vertex.y << " " << vertex.z << " " << (vert % 3 +1) << "\n";
				
				//printf("(%f, %f, %f), ", vertex.x, vertex.y, vertex.z);
			}
			
			//printf("\n");
		}
		pfile.close();
		aiReleaseImport(ploc);
	}
	file.close();
}

void readPowerupfile(char* name)
{
	std::ifstream file;
	file.open(name);

	if(file.is_open())
	{
		while(!file.eof())
		{
			std::string line;
			getline(file, line);

			std::stringstream ss;

			ss << line;

			btScalar x;
			btScalar y;
			btScalar z;
			int type;

			ss >> x;
			ss >> y;
			ss >> z;
			ss >> type;

			btTransform pT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z));

			entManager->createPowerup("model/powerup.lwo", pT, type);
		}
	}
	file.close();
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
				e->read();
				evSys->emitEvent(e);
				delete e;
				break;
			}
		case SDLK_w:
			{
#if SANDBOX
				writeWaypoints("sandboxWaypoints.w");
#elif EDIT_WAYPOINTS
				writeWaypoints("waypoints.w");
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
#if EDIT_WAYPOINTS
		case SDLK_c:
			{
				markConverge();
				break;
			}
		case SDLK_s:
			{
				markWaypointSplit();
				break;
			}
		case SDLK_1:
			{
				saveWaypointIndex1();
				break;
			}
		case SDLK_2:
			{
				savedWaypointIndex2();
				break;
			}
		case SDLK_COMMA:
			{
				connectWaypointsAddition();
				break;
			}
		case SDLK_PERIOD:
			{
				connectWaypointsTruncate();
				break;
			}
		case SDLK_f:
			{
				floatingWaypoint();
				break;
			}
#endif
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
		- updated to not lose the event, but now must pass in controller events*/
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
#if EDIT_WAYPOINTS				
				createWaypoint();
#else		
				if(entManager->getCar(0)->GetPowerUpAt(0)->GetType() != 0)
				{
					entManager->getCar(0)->UsePowerUp(0, false);
					entManager->getCar(0)->RotatePowerups( true );
				}
				//moveWaypoint();
#endif
			}
			if(controller1.isADown())
			{				
				//deleteWaypoint();
				//int i;
				//cin >> i;

				//printf("Trying to use a speed boost...\n");
#if EDIT_WAYPOINTS
				moveWaypoint();
#else
				if(entManager->getCar(0)->GetPowerUpAt(0)->GetType() != 0)
				{
					entManager->getCar(0)->UsePowerUp(0, true);
					entManager->getCar(0)->RotatePowerups( true );
				}
#endif
				//addWaypointInbetween();

			}
			if(controller1.isXDown())
			{
				//entManager->getCar(0)->RotatePowerups( true );
			}
			if(controller1.isYDown())
			{
				//entManager->getCar(0)->RotatePowerups( false );
			}
			if(controller1.isButtonDown(controller1.R_Bump))
			{
#if EDIT_WAYPOINTS
				//moveWaypoint();
				floatingWaypoint();
#else
/*
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
				}*/
				entManager->getCar(0)->RotatePowerups( true );
#endif
			}
			if(controller1.isButtonDown(controller1.L_Bump))
			{/*
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
				}*/
#if EDIT_WAYPOINTS
				addWaypointInbetween();
#else
				entManager->getCar(0)->RotatePowerups( false );
#endif
			}

			if (controller1.isButtonDown(controller1.Start_button))
			{
				/*
#if SANDBOX
				readWaypoints("sandboxWaypoints.w");
#else 				
				readWaypoints("waypoints.w");
#endif
				LoadSoundFile("Documentation/Music/Engine.wav", &EngineSource);
				LoadBackgroundSoundFile("Documentation/Music/InGameMusic.wav");
				*/
				CURRENT_STATE = PAUSED_IN_GAME;
			}
			//if (controller1.isButtonDown(controller1.L_Bump))
			//{
			//	addWaypointInbetween();
			//}

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
		if (c->getPosition().y() < entManager->getTrack()->lowest - 50.f)
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
				c->setNextWaypointIndex(index);
			}
			else
			{
				entManager->resetCar(i, btVector3(0, 3, 0));
			}
		}
		if (c->getNormal().dot(btVector3(0,1,0)) < 0.3 || c->AIresetCounter > 360)
		{
			c->resetCounter++;
			if (c->resetCounter > 40 || c->AIresetCounter > 120)
			{
				int index = getClosestWaypoint(c);
				if (entManager->getWaypointList()->size() > 0 && index != -1)
				{
					Waypoint *w = entManager->getWaypoint(index);						
					btTransform trans = w->getTransform();
					btQuaternion q = btQuaternion(btVector3(0,1,0), trans.getRotation().getAngle() + SIMD_PI);
					trans.setRotation(q);
					entManager->resetCar(i, trans);
					c->setNextWaypointIndex(w->getIndex());
					c->AIresetCounter = 0;
				}
				else
				{
					entManager->resetCar(i, btVector3(0, 3, 0));
				}
				c->resetCounter = 0;
			}
		}
		else
			c->resetCounter = 0;
	}
}

double normalizeNWP(int nwp){
	if(nwp >= 11 && nwp <= 25){
		double a = (nwp - 11) * (20.0 / 14.0);
		return 47 + a;
	}

	if(nwp >= 26 && nwp <= 46){
		double a = (nwp - 26) * (20.0 / 20.0);
		return 47 + a;
	}
		

	if(nwp >= 70 && nwp <= 89){
		double a = (nwp - 70) * (20.0 / 19.0);
		return 115 + a;
	}

	if(nwp >= 90 && nwp <= 114){
		double a = (nwp - 90) * (20.0 / 24.0);
		return 115 + a;
	}

	if(nwp >= 156 && nwp <= 171){
		double a = (nwp - 156) * (37.0 / 15.0);
		return 201 + a;
	}

	if(nwp >= 172 && nwp <= 200){
		double a = (nwp - 172) * (37.0 / 28.0);
		return 201 + a;
	}	

	if(nwp >= 262 && nwp <= 282){
		double a = (nwp - 262) * (33.0 / 20.0);
		return 314 + a;
	}

	if(nwp >= 283 && nwp <= 313){
		double a = (nwp - 283) * (33.0 / 30.0);
		return 314 + a;
	}

	if(nwp >= 394 && nwp <= 418){
		double a = (nwp - 394) * (27.0 / 24.0);
		return 419 + a;
	}

	if(nwp >= 451 && nwp <= 496){
		double a = (nwp - 451) * (57.0 / 45.0);
		return 549 + a;
	}

	if(nwp >= 497 && nwp <= 548){
		double a = (nwp - 497) * (57.0 / 51.0);
		return 549 + a;
	}

	if(nwp >= 617 && nwp <= 659){
		double a = (nwp - 617) * (56.0 / 42.0);
		return 709 + a;
	}

	if(nwp >= 660 && nwp <= 708){
		double a = (nwp - 660) * (56.0 / 48.0);
		return 709 + a;
	}

	if(nwp >= 802 && nwp <= 827){
		double a = (nwp - 802) * (46.0 / 25.0);
		return 864 + a;
	}

	if(nwp >= 828 && nwp <= 863){
		double a = (nwp - 828) * (46.0 / 35.0);
		return 864 + a;
	}

	if(nwp >= 923 && nwp <= 963){
		double a = (nwp - 923) * (57.0 / 40.0);
		return 1021 + a;
	}

	if(nwp >= 964 && nwp <= 1020){
		double a = (nwp - 964) * (57.0 / 56.0);
		return 1021 + a;
	}

	return (double)nwp;
}

void calcPositions(){
	Car* player = entManager->getCar(0);
	int lap = player->lapCount;
	double waypt = normalizeNWP(player->getNextWaypointIndex());
	int position = 1;
	//printf("Player: %d",waypt\n)

	for(int i=1; i<entManager->getCarList()->size(); i++){
		Car* c = entManager->getCar(i);
		int clap = c->lapCount;

		double nwp = c->getNextWaypointIndex();
		nwp = normalizeNWP(nwp);

		if(clap > lap){
			//printf("Their lap is bigger!\n");
			position++;
		}else if(nwp >= waypt && clap == lap){
			//printf("Their wp is bigger!\n");
			position++;
			//printf("%d ",nwp);
		}
	}

	player->currentPosition = position;
}

/*
void calcPositions()
{
	int currentPosition = 6;
	Car* lastCar;
	Car* firstCar;
	int atWaypoint = entManager->numWaypoints();
	int myLapCount = 4;	

	float distanceToWP = 0.0f;

	for (int i = 0; i < entManager->numCars(); i++)
	{
		Car* tempC = entManager->getCar(i);
		tempC->distanceToNextWP = (entManager->getWaypoint(tempC->getNextWaypointIndex())->getPosition() - tempC->getPosition()).dot(entManager->getWaypoint(tempC->getNextWaypointIndex())->getTangent());
		tempC->currentPosition = -1;
	}

	// Find last car 
	for (int i = 0; i < entManager->numCars(); i++)
	{
		Car* tempC = entManager->getCar(i);
		if (tempC->getNextWaypointIndex() <= atWaypoint && tempC->lapCount <= myLapCount)
		{
			if (tempC->getNextWaypointIndex() == atWaypoint)
			{
				if (distanceToWP < tempC->distanceToNextWP)
				{
					lastCar = tempC;
					atWaypoint = lastCar->getNextWaypointIndex();
					myLapCount = lastCar->lapCount;			
					distanceToWP = tempC->distanceToNextWP;
				}
			}
			else {
				lastCar = tempC;
				atWaypoint = lastCar->getNextWaypointIndex();
				myLapCount = lastCar->lapCount;			
				distanceToWP = lastCar->distanceToNextWP;
			}
		}
	}
	lastCar->currentPosition = currentPosition;
	//currentPosition -= 1;
	// Find first car
	atWaypoint = 0;
	myLapCount = 0;
	distanceToWP = 1000.0f;
	for (int i = 0; i < entManager->numCars(); i++)
	{
		Car* tempC = entManager->getCar(i);
		if (tempC->getNextWaypointIndex() >= atWaypoint && tempC->lapCount >= myLapCount)
		{
			if (tempC->getNextWaypointIndex() == atWaypoint)
			{
				if (distanceToWP > tempC->distanceToNextWP)
				{
					firstCar = tempC;
					atWaypoint = lastCar->getNextWaypointIndex();
					myLapCount = lastCar->lapCount;			
					distanceToWP = tempC->distanceToNextWP;
				}
			}
			firstCar = tempC;
			atWaypoint = firstCar->getNextWaypointIndex();
			myLapCount = firstCar->lapCount;			
			distanceToWP = tempC->distanceToNextWP;
		}
	}
	firstCar->currentPosition = 1;


	//Last Found Car: last car that got it's position found.
	Car* nextCar = lastCar;
	Car* frontCar = firstCar;
	atWaypoint = lastCar->getNextWaypointIndex();
	myLapCount = lastCar->lapCount;
	distanceToWP = lastCar->distanceToNextWP;
	for (int i = 0; i < entManager->numCars() - 1; i++)
	{		
		for (int j = 0; j < entManager->numCars(); j++)
		{
			Car* tempC = entManager->getCar(j);
			//Infront of last car.
			if (tempC->getNextWaypointIndex() <= frontCar->getNextWaypointIndex())
			{
				if (tempC->lapCount <= frontCar->lapCount)
				{
					if (tempC->getNextWaypointIndex() == frontCar->getNextWaypointIndex())
					{
						if (tempC->distanceToNextWP > frontCar->distanceToNextWP)
						{
							if (tempC->id != nextCar->id && tempC->currentPosition == -1)
								frontCar = tempC;
						}
					}
					else
					{
						if (tempC->id != nextCar->id && tempC->currentPosition == -1)
								frontCar = tempC;
					}
				}
			}
		}
		nextCar = frontCar;
		frontCar = firstCar;
		nextCar->currentPosition = currentPosition;
		currentPosition -= 1;
	}

	
	//for (int i = 0; i < entManager->numCars() - 1; i++)
	//{
	//	//Last Found Car: last car that got it's position found.
	//	Car* lastFoundCar = firstCar;
	//	for (int j = 0; j < entManager->numCars(); j++)
	//	{
	//		currentCar = entManager->getCar(j);

	//		if (currentCar->getNextWaypointIndex() >= lastCar->getNextWaypointIndex() &&
	//			currentCar->lapCount >= lastCar->lapCount)
	//		{				
	//			if (currentCar->getNextWaypointIndex() <= lastFoundCar->getNextWaypointIndex()
	//				&& currentCar->lapCount <= lastFoundCar->lapCount)
	//			{
	//				if (currentCar->getNextWaypointIndex() != lastFoundCar->getNextWaypointIndex() || 
	//					(lastFoundCar->distanceToNextWP < currentCar->distanceToNextWP) && currentCar->getNextWaypointIndex() == lastFoundCar->getNextWaypointIndex())
	//				{
	//					lastFoundCar = currentCar;					
	//				}
	//			}
	//		}
	//	}
	//	lastFoundCar->currentPosition = currentPosition;
	//	currentPosition -= 1;
	//	//Car behind the next postion to be found.
	//	lastCar = lastFoundCar;
	//	lastFoundCar = firstCar;
	//}
}
*/

// Engine Main
int main(int argc, char** argv)
{	
	// INITIALIZATIONS
	//int BLAHDEBLAH = 0;
	
	//Initialize the renderer
	bool renInit = ren->init();
	ph->initObserver();

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

	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	while (stillWantsToPlay)
	{
/* Menu Code */
	Menu m = Menu();	
	srand( time(NULL) );

	float ListenerPosition[3] = {0.0, 0.0, 0.0};
	soundPlayer.LoadSoundFile("Documentation/Music/Main Menu Music.wav", MenuSource, MenuBuffer, ListenerPosition, AL_TRUE);
	alSourcef(MenuSource, AL_GAIN, 0.5f );
	
	//loadPowerupLocation("model/poweruplocation.lwo");
	int selection = m.run(ren);
	if (selection == m.QUIT)
	{
		exit(0);
	}
	if (selection == m.START)
	{
		CURRENT_STATE = LOADING_GAME;
		m.loading(ren, "PowerUps");
		readPowerupfile("poweruploc.p");
	}
	// //RENDERER DEBUG TESTING
	
	// Create all the enitities.
	btScalar carMass = 1.5;

	btTransform groundT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -5, 0));

	/*	
	for(int i = 1; i < 5; i++){
		btTransform powerupT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.f, 5.5f, 50.f*i));
		entManager->createPowerup("model/powerup.lwo", powerupT1, 1);
	}

	

	for(int i = 1; i < 10; i++)
	{
		btTransform powerupT1 = btTransform(btQuaternion(0, 0, 0, 1), btVector3(-50.f*i, 5.5f, 450.f));
		entManager->createPowerup("model/powerup.lwo", powerupT1, 2);
	}
	*/
	

	
	m.loading(ren, "Track");
	
#if SANDBOX
	entManager->createTrack("model/groundBox.lwo", groundT);
#else
	entManager->createTrack("model/Track.lwo", groundT);
#endif

m.loading(ren, "AI Information");
	readWaypoints("waypoints.w");

m.loading(ren, "Cars");

	//entManager->createCar("model/ship1.lwo", carMass, carT1);	
	//entManager->createCar("model/ship1.lwo", carMass, carT2);	
	//btTransform carT3 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(0.0f, 3.0f, -30.0f));	
	//	entManager->createCar("model/ship1.lwo", carMass, carT3);		

/*
	for(int i = 4; i > 1; i--){
		btTransform carT3 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(0.0f, 3.0f, (float)i*-30.0f));	
		entManager->createCar("model/Ship1.lwo", carMass, carT3);	
		btTransform carT4 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(30.0f, 3.0f, (float)i*-30.0f));	
		entManager->createCar("model/ship1.lwo", carMass, carT4);	
	}
*/

	btTransform carT1 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(15.0f, 3.0f, 0.f));	
	btTransform carT2 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(-15.0f, 3.0f, -15.f));	
	btTransform carT3 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(15.0f, 3.0f, -30.f));	
	btTransform carT4 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(-15.0f, 3.0f, -45.f));	
	btTransform carT5 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(15.0f, 3.0f, -60.f));	
	btTransform carT6 = btTransform(btQuaternion(0, 1, 0, 1), btVector3(-15.0f, 3.0f, -75.f));	

	entManager->createCar("model/Ship1.lwo", carMass, carT6);
	entManager->createCar("model/Ship2.lwo", carMass, carT5);
	entManager->createCar("model/Ship3.lwo", carMass, carT4);
	entManager->createCar("model/Ship4.lwo", carMass, carT3);
	entManager->createCar("model/Ship5.lwo", carMass, carT2);
	entManager->createCar("model/Ship6.lwo", carMass, carT1);

	for (int i = 0; i < entManager->getCarList()->size(); i++)
		entManager->getCar(i)->setNextWaypointIndex(getClosestWaypoint(entManager->getCar(i)) + 2);

	m.loading(ren, "Sky");
	btTransform skyT = btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0));
	entManager->createSky("model/skydome2.lwo", skyT);

	m.loading(ren, "Initializing Variables,\n Almost done!");
	// Variables for lap time
	int LapMinutes = 0;
	int LapSeconds = 0;
	int LapMilliseconds = 0;
	int totalMinutes = 0;
	int totalLapSeconds = 0;
	int totalLapMilliseconds = 0;
	
	//Initialize camera settings.
	btVector3 car1N = entManager->getCar(0)->getNormal()*10;
	btVector3 car1T = entManager->getCar(0)->getBinormal()*30;
	
	btVector3 camOffset = car1N + car1T;
	btVector3 camLookAt = entManager->getCar(0)->getPosition();
	camera1.setUpCamera(camLookAt, camOffset);
	camera1.setTrackCar(entManager->getCar(0));		

	//Load variables from the xml file.
	ReloadEvent* e = new ReloadEvent();
	e->read();
	evSys->emitEvent(e);
	delete e;

	float EngineModifier = 0;
	
	int LapNumber = entManager->getCar(0)->lapCount;
	int WaypointIndex = -1;
	int CurrentWaypointIndex = 0;
	int finishingPosition = 0;

	//Set inital game time
	Uint32 currentTime = SDL_GetTicks();
	Uint32 oldTime = SDL_GetTicks();
	Uint32 physicsCurrentTime = SDL_GetTicks();
	Uint32 physicsOldTime = SDL_GetTicks();
	int frameCount = 0;
	int AIpowerUPDelayCounter = 0;
	int counter = 1;
	int instantFrameCount = 0;
	int CarSpeed = 0;
	stringstream instantFrameCountBuffer;
	string instantFrameString = "";
	string CarSpeedString = "";

	int loops;
	float interpolation;
	bool running = true;

	Uint32 next_game_tick = SDL_GetTicks();	

	alSourceStop(MenuSource);

	m.loading(ren, "Game Ready!\nPress Start To Continue", true);

	/*Load game music */
	float ListenerPos[3] = { entManager->getCar(0)->getPosition().x(), entManager->getCar(0)->getPosition().y(), entManager->getCar(0)->getPosition().z() };
	soundPlayer.LoadSoundFile("Documentation/Music/Engine.wav", EngineSource, EngineBuffer, ListenerPos, AL_TRUE);
	soundPlayer.LoadSoundFile("Documentation/Music/InGameMusic.wav", BackgroundSource, BackgroundBuffer, ListenerPos, AL_TRUE);
	alSourcef(BackgroundSource, AL_GAIN, 0.15f );
	alSourcef(EngineSource, AL_GAIN, 0.4f );
	//soundPlayer.LoadBackgroundSoundFile("Documentation/Music/InGameMusic.wav");

	// game loop
	CURRENT_STATE = GAME_STARTING;
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
			
		while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP && CURRENT_STATE == GAME_RUNNING)
		{
			float ListenerPos[3] = { entManager->getCar(0)->getPosition().x(), entManager->getCar(0)->getPosition().y(), entManager->getCar(0)->getPosition().z() };
			soundPlayer.UpdateListenerPosition( ListenerPos );

			for( int i = 0; i < entManager->numCars() ; i++ )
			{
				entManager->getCar(i)->ListenerPosition[0] = ListenerPos[0];
				entManager->getCar(i)->ListenerPosition[1] = ListenerPos[1];
				entManager->getCar(i)->ListenerPosition[2] = ListenerPos[2];
			}

			// Calculate engine's change in pitch
			EngineModifier = (entManager->getCar(0)->GetSpeed() / (-1 * entManager->getCar(0)->GetForwardForceModifier()));

			// If going in reverse, we still want engine to rev up
			if(EngineModifier < 0)
				EngineModifier *= -1;

			// Change pitch of engine sound
			alSourcef(EngineSource, AL_GAIN, 0.4f + (EngineModifier / 2.0) );
			alSourcef(EngineSource, AL_PITCH, 1.0f + EngineModifier );
			alSourcef(BackgroundSource, AL_GAIN, 0.15f + (EngineModifier / 1.75) );
			
			//// Physics
			physicsCurrentTime = SDL_GetTicks();
			btScalar dif = btScalar(physicsCurrentTime - physicsOldTime);
			//printf("DIFF = %f\n" , dif);
			//btScalar phyTS(1.f/(float)(physicsCurrentTime-physicsOldTime));
			btScalar phyTS(1.f/60.f);
			ph->step(phyTS);
			physicsOldTime = physicsCurrentTime;

			//// Inputs
			process_events();
			
			controller1.emitTriggers();
			controller1.emitButtons();
			controller1.emitLeftAnalog();
			controller1.emitRightAnalog();

			//// AI
			//// Calculate current lap for player's car
			for (int i = 0 ; i < entManager->getCarList()->size(); i++)
			{
				Car* tempCarPtr = entManager->getCar(i);
				int currentWPIndex = tempCarPtr->getNextWaypointIndex();
				if (currentWPIndex > entManager->getWaypointList()->size()/2 && currentWPIndex < entManager->getWaypointList()->size()/2 + 200)
				{
					tempCarPtr->halfWayAround = true;
				}
				if (tempCarPtr->halfWayAround && currentWPIndex < 15 && currentWPIndex > 5 && !tempCarPtr->finishedRacing)
				{
					tempCarPtr->halfWayAround = false;
					tempCarPtr->lapCount++;
					if( entManager->getCar(0)->lapCount == 3 )
						alSourcef(BackgroundSource, AL_PITCH, 1.05f );
					tempCarPtr->finishedLap(totalMinutes, totalLapSeconds, totalLapMilliseconds);
					if (tempCarPtr->lapCount == 4)
					{																	
						tempCarPtr->finishedRacing = true;
						//finishingPosition++;
						//tempCarPtr->finalPosition = finishingPosition;
						tempCarPtr->displayTime();
						/*Modification to stop after first human player CHANGE FOR MULTIPLAYER IF ADDED*/
						if (tempCarPtr->id == 0)
							CURRENT_STATE = GAME_FINISHED;
					}
					if (tempCarPtr->id == 0)
					{
						LapMilliseconds = 0;
						LapMinutes = 0;
						LapSeconds = 0;
						LapNumber++;
					}
				}
			}

			AIpowerUPDelayCounter++;
			ai->generateNextMove();
			calcPositions();
			if (AIpowerUPDelayCounter > 240)
			{
				for(int i = 0; i < entManager->numCars(); i++)			
					entManager->getCar(i)->usedPowerUpRecently = false;
			}
			entManager->getCar(0)->setNextWaypointIndex(getClosestWaypoint());
			
			/*if(BLAHDEBLAH == 0){
				for(int i=0; i<entManager->getWaypointList()->size(); i++){
					if(entManager->getWaypoint(i)->split == true){
						printf("%d splits with children:\n",entManager->getWaypoint(i)->getIndex());
						for(int j=0; j<entManager->getWaypoint(i)->getWaypointList().size(); j++){
							printf("%d, ",entManager->getWaypoint(i)->getWaypointList().at(j)->getIndex());
						}
						printf("\n");
					}
					if(entManager->getWaypoint(i)->converge == true){
						printf("%d Converges.\n",entManager->getWaypoint(i)->getIndex());
					}
				}
				BLAHDEBLAH = 1;
			}*/

			// Resets any cars which have fallen off the track.
			resetCars();

			if (CURRENT_STATE == GAME_FINISHED)
			{			
				Car* playerCar = entManager->getCar(0);
				//for every car
				for (int i = 0; i < entManager->numCars(); i++)
				{
					//fetch the car
					Car* tempC = entManager->getCar(i);

					//if it's not finished racing
					if (tempC->finishedRacing == false)
					{
						//initialize some things
						int tempMin=0, tempSec=0, tempMil =0;

						//for every lap
						for (int j = 0; j < 3; j++)
						{
							//if they finished the lap, collect the data, else fabricate it
							if (tempC->lapTimes.size() > j)
							{
								tempMin += tempC->lapTimes.at(j).min;
								tempSec += tempC->lapTimes.at(j).sec;
								tempMil += tempC->lapTimes.at(j).mil;
								continue;
							}
							else
							{
								tempMin += playerCar->lapTimes.at(j).min;
								tempSec += playerCar->lapTimes.at(j).sec;
								tempMil += playerCar->lapTimes.at(j).mil;

								//tempSec += tempSec % 35;
								int randm = rand() % 41 + 3;
								tempSec += randm;
								printf("Adding %d",randm);
								while (tempMil >= 1000)
								{
									tempMil = tempMil - 1000;
									tempSec = tempSec + 1;
								}
								while (tempSec >= 60)
								{
									tempSec = tempSec - 60;
									tempMin = tempMin + 1;
								}
								tempC->finishedLap(tempMin, tempSec, tempMil);
							}
						}
					}else
					{
						/*Note: This code shouldn't end up getting run anymore */
						if(tempC->lapTimes.size() == 1){
							int tempMin=0, tempSec=0, tempMil =0;
							for (int p = 0; p < tempC->lapTimes.size(); p++)
							{
								tempMin += tempC->lapTimes.at(p).min;
								tempSec += tempC->lapTimes.at(p).sec;
								tempMil += tempC->lapTimes.at(p).mil;
							}
							tempC->lapTimes.resize(0);
							//tempC->lapTimes.re
							int totalSec = tempMin * 60 + tempSec;
							int lapSecs = totalSec /3;
							int avgMin = lapSecs /60;
							int avgSec = lapSecs % 60;
							printf("Hacking time: ID: %d Time: %d : %d\n", tempC->id, avgMin, avgSec);
							
							for (int p = 1; p <4; p++){
								tempC->finishedLap(p*avgMin, p*avgSec, 0);
								
							}

						}
					}
				}

				//for every car
				for (int i = 0; i < entManager->numCars(); i++)
				{
					//get the car
					Car* tempC = entManager->getCar(i);
					int pos = 1;
					int totalSecC = tempC->totalMin * 60 + tempC->totalSec;
					//for every car (again)
					for (int j = 0; j < entManager->numCars(); j++)
					{
						Car* tempCompare = entManager->getCar(j);
						int totalSecCompare = tempCompare->totalMin * 60 + tempCompare->totalSec;
						if (totalSecCompare < totalSecC)
							pos++;
					}
					tempC->finalPosition = pos;																	
					printf("ID: %d Position: %d\n", tempC->id, tempC->finalPosition);
				}
				int currentPosToFind = 1;
				int count = 0;
				for (int j = 0; j < entManager->numCars(); j++)
				{
					count = 0;
					for (int p = 0; p < entManager->numCars(); p++)
					{
						Car* tempCompare = entManager->getCar(p);
						if (currentPosToFind == tempCompare->finalPosition)
							count++;
					}
					while (count > 1)
					{
						for (int p = 0; p < entManager->numCars() && count > 1; p++)
						{
							Car* tempCompare = entManager->getCar(p);
							if (currentPosToFind == tempCompare->finalPosition)
							{
								count--;
								tempCompare->finalPosition++;
							}
						}							
					}
					currentPosToFind++;
				}
				for (int j = 0; j < entManager->numCars(); j++)
				{
					Car* tempC = entManager->getCar(j);
					tempC->displayTime();
				}
				m.timeScreen(ren);
				alSourceStop(BackgroundSource);
				alSourceStop(EngineSource);
				running = false;
			}
			//WaypointIndex = entManager->getCar(0)->getNextWaypointIndex();
			//if( WaypointIndex != CurrentWaypointIndex )
			//{
			//	CurrentWaypointIndex = WaypointIndex;
			//	if( CurrentWaypointIndex == 0 )
			//	{
			//		LapNumber++;
			//		LapMinutes = 0;
			//		LapSeconds = 0;
			//		LapMilliseconds = 0;
			//	}
			//}

			next_game_tick += SKIP_TICKS;
			loops++;
			instantFrameCount++;
		}

		//interpolation = float(SDL_GetTicks() + SKIP_TICKS - next_game_tick/ float(SKIP_TICKS));

		// Render
		ren->clearGL();	// clear the screen

		// done in light space
		ren->depthMapPass();
		// this will blur the depth map, the final blurred depth map will be in "smap"
		ren->blurPass();
		camera1.updateCamera(entManager->getCar(0)->physicsObject->getWorldTransform());

		ren->drawSky(camera1);

		ren->clearGL();	// clear the screen
		ren->drawShadow(camera1);
/*
		ren->glEnable2D();
		ren->clearGL();	// clear the screen
		ren->drawTexture("sky2D");		
		ren->glDisable2D();
*/		
		
/*
		ren->clearGL();		
		ren->setCamera(camera1);
		ren->drawAll();
		ren->drawTrack();
*/	
//		ren->drawAll();
		ren->glDisableLighting();
		ph->debugDraw();
		ren->glEnableLighting();

		ren->glEnable2D();
		glColor4f(1.0, 1.0, 1.0, 1.0);

		/* Starting CountDown */
		if (CURRENT_STATE == GAME_STARTING)
		{
			ren->changeFontSize(56);
			if (counter == 2)
				ren->outputText("3", 255, 0,255, 1280/2, 780/2);
			else if (counter == 3)
				ren->outputText("2", 255, 0,255, 1280/2, 780/2);
			else if (counter == 4)
				ren->outputText("1", 255, 0,255, 1280/2, 780/2);
			else if (counter == 5)
			{
				ren->outputText("GO!", 255, 255,0, 1280/2, 780/2);
				CURRENT_STATE = GAME_RUNNING;
				next_game_tick = SDL_GetTicks();
			}
		}
		if (CURRENT_STATE == PAUSED_IN_GAME)
		{
			int selection = m.inGameMenu(ren);
			if (selection == m.CONTINUE)
			{
				CURRENT_STATE = GAME_RUNNING;
				next_game_tick = SDL_GetTicks();
			}
			else if (selection = m.QUIT_IN_GAME)
				running = false;
		}

		ren->changeFontSize(20);
		//ren->outputText(entManager->getCarList()->at(0)->toString(), 255, 255, 255, 200, 200);

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
			totalLapSeconds++;
		}		
		currentTime = SDL_GetTicks();

		//ren->outputText("FPS: " + instantFrameString, 0, 255, 0, 0, 680);

		std::stringstream ssSpeed;
		CarSpeed = (int)(EngineModifier * 750.0f);
		if( CarSpeed < 0 )
			CarSpeed = 0;
		ssSpeed << CarSpeed;
		CarSpeedString = ssSpeed.str();
		ren->changeFontSize(50);
		ren->outputText(CarSpeedString, 0, 255, 0, 1020, 60);
		ren->changeFontSize(20);
		ren->outputText("km/h", 0, 255, 0, 1150, 60);

		//for (int i = 0; i < entManager->getSpawnableList()->size(); i++)
		//{
		//	Rocket* r = dynamic_cast<Rocket*>(entManager->getSpawnableList()->at(i));
		//	if (r == NULL)
		//		continue;
		//	r->applyNextMove();
		//	printf(r->toString().c_str());
		//}
		
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
		ssLap << entManager->getCar(0)->lapCount;

		// Calculate the current lap time
		if( LapMinutes < 10 )
		{
			ssLapTime << "0";			
		}
		ssLapTime << LapMinutes << ":";

		if (totalLapSeconds >= 60)
		{
			totalMinutes++;
			totalLapSeconds = 0;
		}

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

		//Display Position
		ssLapTime << "\nCurrent Position: " << entManager->getCar(0)->currentPosition << " / 6";

		// Display the current lap time
		ren->outputText("Current Lap: " + ssLapTime.str(), 255, 0, 0, 0, 660);
		ren->outputText("Lap: " + ssLap.str(), 255, 0, 0, 0, 640);

		/*
		std::stringstream ssPowerUps;
		for( int i = 0; i < 3; i++ )
		{
	
			if( entManager->getCar(0)->GetPowerUpAt(i)->GetType() == 0 )
				ssPowerUps << "Empty ";
			else if( entManager->getCar(0)->GetPowerUpAt(i)->GetType() == 1 )
				ssPowerUps << "|| Slow / Speed ||";
			else if( entManager->getCar(0)->GetPowerUpAt(i)->GetType() == 2 )
				ssPowerUps << "|| Rocket / Shield ||";
			else if( entManager->getCar(0)->GetPowerUpAt(i)->GetType() == 3 )
				ssPowerUps << "|| Mine / Nova ||";			
		}

		//ren->outputText("Powerups: " + ssPowerUps.str(), 255, 0, 0, 300, 700);
		*/

		if(entManager->numWaypoints() > 0)
		{
			Car* player = entManager->getCar(0);
			int wayPoint = player->getNextWaypointIndex();

			Waypoint* wp = entManager->getWaypoint(wayPoint);
			
			btVector3 ptan = player->getTangent();
			btVector3 wtan = wp->getTangent();

			if(ptan.dot(wtan) > 0)
			{
				ren->changeFontSize(50);
				ren->outputText("Wrong Way", 255, 0, 0, 500, 600);
			}
		}

		glColor4f(1.0, 1.0, 1.0, 1.0);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ren->drawTexture("hud");

		// this will draw the middle powerup
		btVector3 c11 = btVector3(117, 317, 0);
		btVector3 c21 = btVector3(203, 403, 0);
		glActiveTexture(GL_TEXTURE0);
		glColor4f(1, 1, 1, 1);
		// slow/speed
		if( entManager->getCar(0)->GetPowerUpAt(0)->GetType() == 1 )
		{	
			ren->textureOn(ren->getTexture("ss"));
		}
		// rocket/shield
		else if( entManager->getCar(0)->GetPowerUpAt(0)->GetType() == 2 )
		{
			ren->textureOn(ren->getTexture("rs"));
		}
		// nova/mine
		else if( entManager->getCar(0)->GetPowerUpAt(0)->GetType() == 3 )
		{
			ren->textureOn(ren->getTexture("mn"));
		}
		// empty
		else
		{
			ren->textureOff();
			glColor4f(0, 0, 0, 0);
		}
		ren->drawQuad(c11, c21);


		// this will draw the left powerup
		btVector3 c12 = btVector3(40, 452, 0);
		btVector3 c22 = btVector3(126, 538, 0);
		glActiveTexture(GL_TEXTURE0);
		glColor4f(1, 1, 1, 1);
		// slow/speed
		if( entManager->getCar(0)->GetPowerUpAt(1)->GetType() == 1 )
		{
			ren->textureOn(ren->getTexture("ss"));
		}
		// rocket/shield
		else if( entManager->getCar(0)->GetPowerUpAt(1)->GetType() == 2 )
		{
			ren->textureOn(ren->getTexture("rs"));
		}
		// nova/mine
		else if( entManager->getCar(0)->GetPowerUpAt(1)->GetType() == 3 )
		{
			ren->textureOn(ren->getTexture("mn"));
		}
		// empty
		else
		{
			ren->textureOff();
			glColor4f(0, 0, 0, 0);
		}
		ren->drawQuad(c12, c22);


		// this will draw the right powerup
		btVector3 c13 = btVector3(40, 181, 0);
		btVector3 c23 = btVector3(126, 267, 0);
		glActiveTexture(GL_TEXTURE0);
		glColor4f(1, 1, 1, 1);
		// slow/speed
		if( entManager->getCar(0)->GetPowerUpAt(2)->GetType() == 1 )
		{
			ren->textureOn(ren->getTexture("ss"));
		}
		// rocket/shield
		else if( entManager->getCar(0)->GetPowerUpAt(2)->GetType() == 2 )
		{
			ren->textureOn(ren->getTexture("rs"));
		}
		// nova/mine
		else if( entManager->getCar(0)->GetPowerUpAt(2)->GetType() == 3 )
		{
			ren->textureOn(ren->getTexture("mn"));
		}
		// empty
		else
		{
			ren->textureOff();
			glColor4f(0, 0, 0, 0);
		}
		ren->drawQuad(c13, c23);

		ren->glDisable2D();

		glColor4f(1,1,1,1);

		ren->updateGL();	// update the screen

	
	}
	running = true;

	/*delete ph;
	ph = Physics::Inst();*/
	//delete entManager;
	m.freeingScreen(ren, "Releasing Entities...");
	entManager->clean();// = EntityManager::getInstance();
	entManager = EntityManager::getInstance();	
	//evSys->clean(&ph->variableObserver);	
	m.freeingScreen(ren, "Releasing Physics...");
	MethodObserver<ReloadEvent, Physics> tempObs = ph->variableObserver;
	ph->clean();
	ph = Physics::Inst();
	//tempObs.objectInstance = ph;
	//tempObs.funcPointer = &(Physics::variableObserver);
	//ph->variableObserver = tempObs;
	ph->initObserver();
	// DEBUG DRAW SETUP
	ph->setDebugDrawer(ren);	
	ph->setDebugLevel(btIDebugDraw::DBG_NoDebug);	// DRAW EVERYTHING
	controller1.initialize(0);
	//entManager->getCar(0)->initObservers();
}
	return 0;
}