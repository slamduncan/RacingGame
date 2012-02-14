#ifndef AIHANDLER_H
#define AIHANDLER_H

#include "EntityManager.h"
#include "Waypoint.h"
#include "Car.h"
#include "LinearMath\btAlignedObjectArray.h"

class AIHandler {

private:	
	btAlignedObjectArray<Waypoint*>* waypoints;
	btAlignedObjectArray<Car*>* cars;
	static AIHandler* instance;
	AIHandler();

public:
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