#ifndef ROCKET_H
#define ROCKET_H

#include "Spawnable.h"
#include "Car.h"
#include "Waypoint.h"
#include "EntityManager.h"
#include "MethodObserver.h"

class Rocket : public Spawnable {

public:	
	void applyNextMove();
	int getNextWaypointIndex();
	Rocket(int startingWaypointIndex, int spawnedBy);
	void setNextWaypointIndex(int index);
	void reloadVariables(ReloadEvent *e);
private:
	Car* getClosestCar(bool inFront, btScalar &detectionRange);
	int nextWaypoint;
	btAlignedObjectArray<Waypoint*>* waypoints;
	btScalar lastAngleForce;
	void positionCheck(Waypoint* w);
	float turningModifier;
	float forwardModifier;
	float maxMovementForce;
	float rateOfChangeModifier;
	float rocketSpeed;
	float detectionRange;	
	//MethodObserver<ReloadEvent, Rocket> reloadObserver;
};

#endif