#include "AIHandler.h"

AIHandler* AIHandler::instance = 0;

AIHandler::AIHandler() : reloadObserver(this, &AIHandler::reloadVariables){
	waypoints = EntityManager::getInstance()->getWaypointList();
	cars = EntityManager::getInstance()->getCarList();
	turningModifier = 1.0;
	forwardModifier = 1.0;
	maxMovementForce = 1.0;
	reloadObserver.init(EventTypes::RELOAD_VARIABLES);
}

void AIHandler::generateNextMove(){
	for(int i = 1; i < cars->size(); i++){
		Car* c = cars->at(i);
		int waypointIndex = c->getNextWaypointIndex();
		if (waypointIndex == -1){
			return;
		}
		Waypoint* w = waypoints->at(waypointIndex);
		w->positionCheck(c);
		while (waypointIndex != c->getNextWaypointIndex())
		{			
			Waypoint* w = waypoints->at(waypointIndex);
			w->positionCheck(c);
			waypointIndex = c->getNextWaypointIndex();
		}

		btVector3 carPos =  c->getPosition();
		btVector3 wayPos = w->getPosition();
		btVector3 tan = c->getTangent();
		btVector3 toWaypoint = wayPos - carPos;		
		btVector3 angleRot = toWaypoint -  tan.normalized();
		btScalar turningScalar = angleRot.dot(c->getBinormal());
		btScalar rateOfChange = c->lastAngleForce - turningScalar;

		btScalar roationForce = turningScalar*turningModifier - rateOfChange*rateOfChangeModifier;
		c->lastAngleForce = turningScalar;

		btScalar distance = toWaypoint.length();
		btScalar forwardForce = btScalar(-distance*forwardModifier);
		//btScalar forwardForce = btScalar(w->getThrottle());
		if(roationForce > maxMovementForce)
			roationForce = maxMovementForce;
		else if(roationForce < -maxMovementForce)
			roationForce = - maxMovementForce;

		RotationEvent* re = new RotationEvent(btQuaternion(0, roationForce,0,0));
		c->observeRotation(re);		
		delete re;
		if (forwardForce > maxMovementForce)
			forwardForce = maxMovementForce;
		else if (forwardForce < -maxMovementForce)
			forwardForce = -maxMovementForce;
		ForwardForceEvent* ffe = new ForwardForceEvent(forwardForce, forwardForce/32767.0);
		c->observeForwardForce(ffe);
		delete ffe;
		//printf("Car: %d, Forward = %f, Turn = %f\n", i, forwardForce, roationForce);
	}
}

void AIHandler::reloadVariables(ReloadEvent *e){
	turningModifier = e->numberHolder.aiInfo.rotateModifier;
	forwardModifier = e->numberHolder.aiInfo.drivingModifier;
	maxMovementForce = e->numberHolder.aiInfo.maxMovementForce;
	rateOfChangeModifier = e->numberHolder.aiInfo.rateOfChangeModifier;
}