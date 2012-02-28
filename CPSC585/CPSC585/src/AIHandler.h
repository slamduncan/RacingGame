#ifndef AIHANDLER_H
#define AIHANDLER_H

#define THREE_QUARTER_PI btScalar(SIMD_HALF_PI * 3.0)
#include "EntityManager.h"
#include "Waypoint.h"
#include "Car.h"
#include "LinearMath\btAlignedObjectArray.h"
#include "ReloadEvent.h"
#include "MethodObserver.h"

class AIHandler {

private:	
	btAlignedObjectArray<Waypoint*>* waypoints;
	btAlignedObjectArray<Car*>* cars;
	static AIHandler* instance;
	AIHandler();
	MethodObserver<ReloadEvent, AIHandler> reloadObserver;
	float turningModifier;
	float forwardModifier;
	float maxMovementForce;

public:
	void reloadVariables(ReloadEvent *e);
	void generateNextMove();
	static AIHandler* getInstance()
	{
		if(instance == NULL)
		{
			instance = new AIHandler();
		}

		return instance;
	}
};
#endif