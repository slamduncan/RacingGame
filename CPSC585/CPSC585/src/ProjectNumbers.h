//#define TIXML_USE_STL
#ifndef PROJECTNUMBERS_H
#define PROJECTNUMBERS_H

#include <fstream>

#include "tinyxml.h"
#include "tinystr.h"
#include "LinearMath/btVector3.h"


class ProjectNumbers {

public:
	struct controller{		
		float rotateModifier; //InputMapper
	} controllerInfo;

	struct physics{
		float cModifier; //Car
		float kModifier; //Car
		float forwardForceModifier; //Car 
		btVector3 gravity; //Physics
		float staticFrictionModifier; //Physics
		float dynamicFrictionModifier; //Physics
		float sideFrictionModifier; //Car
		float forwardFrictionModifier; //Car
		float turningForceModifier; //Car
		float springForceModifier; //Car
		float speedBoostModifier;  //Car
		float slowFieldModifier; //Car
		float rocketModifier; //Car
		float tractionBoostModifier; //Car
		float shieldModifier; //Car
		float forceBubbleModifier ; //Car
	} physicsInfo;

	struct AI{
		float rotateModifier; //AiHandler
		float drivingModifier; //AiHandler
		float maxMovementForce; //AiHandler
		float goToNextWaypointDistanceBefore; //AiHandler
		float goToNextWaypointDistanceAfter; //AiHandler
		float rateOfChangeModifier; //AiHandler
		float carDetectionRange; //Car
	} aiInfo;

	ProjectNumbers(){}//readVariablesIn();}


	void readVariablesIn(){
		readPhysics();
		readController();
		readAI();
	}

	void readPhysics();
	void readController();
	void readAI();

};
#endif