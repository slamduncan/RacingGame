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
		Car* closeC = c->getClosestCar(true);

		btScalar forwardForce, turningScalar, rateOfChange, roationForce, distance;
		btVector3 carPos, wayPos, tan, toWaypoint, angleRot;

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

		if( i == 0 )
			continue;

		carPos =  c->getPosition();
		if(closeC != NULL)
			wayPos = closeC->getPosition();
		else
			wayPos = w->getPosition();
		tan = c->getTangent();
		toWaypoint = wayPos - carPos;		
		angleRot = toWaypoint -  tan.normalized();
		turningScalar = angleRot.dot(c->getBinormal());
		rateOfChange = c->lastAngleForce - turningScalar;

		roationForce = turningScalar*turningModifier - rateOfChange*rateOfChangeModifier;
		c->lastAngleForce = turningScalar;

		distance = toWaypoint.length();
		forwardForce = btScalar(-distance*forwardModifier);
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
		ForwardForceEvent* ffe = new ForwardForceEvent(forwardForce, forwardForce/32767.0f);
		c->observeForwardForce(ffe);
		delete ffe;
		//printf("Car: %d, Forward = %f, Turn = %f\n", i, forwardForce, roationForce);
	}
}

void AIHandler::PowerUpCheck(Car* c)
{
	for (int i= 0; i < 3; i++)
	{
		PowerUp* p = c->GetPowerUpAt(i);	
		bool carInFront = c->getClosestCar(true) != NULL;
		bool carBehind = c->getClosestCar(false) != NULL && carInFront == false;
		switch(p->GetType())
		{
		case(ROCKET_SHIELD):
			{
				if (carInFront)
					c->UsePowerUp(i, true);
			}
		case(SPEED_SLOW):
			{
				if (carBehind)
				{
					c->UsePowerUp(i, true);
				}
			}
		}
		
	}
}

void AIHandler::reloadVariables(ReloadEvent *e){
	turningModifier = e->numberHolder.aiInfo.rotateModifier;
	forwardModifier = e->numberHolder.aiInfo.drivingModifier;
	maxMovementForce = e->numberHolder.aiInfo.maxMovementForce;
	rateOfChangeModifier = e->numberHolder.aiInfo.rateOfChangeModifier;
}