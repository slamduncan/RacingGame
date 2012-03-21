#include "Rocket.h"

Rocket::Rocket(int startingWaypoint) : reloadObserver(this, &Rocket::reloadVariables)
{
	nextWaypoint = startingWaypoint;
	waypoints = EntityManager::getInstance()->getWaypointList();
	lastAngleForce = btScalar(0);
	turningModifier = 1.0;
	forwardModifier = 1.0;
	maxMovementForce = 1.0;
	reloadObserver.init(EventTypes::RELOAD_VARIABLES);
}


void Rocket::applyNextMove()
{
	btScalar detectionRng(100);
	Car* closeC = getClosestCar(true, detectionRng);
	btScalar forwardForce, turningScalar, rateOfChange, roationForce, distance;
	btVector3 rocketPos, wayPos, tan, toWaypoint, angleRot;
	int waypointIndex = getNextWaypointIndex();
	if (waypointIndex == -1){
		return;
	}
	Waypoint* w = waypoints->at(waypointIndex);
	positionCheck(w);
	while (waypointIndex != getNextWaypointIndex())
	{			
		Waypoint* w = waypoints->at(waypointIndex);
		positionCheck(w);
		waypointIndex = getNextWaypointIndex();
	}
	rocketPos = getPosition();
	if(closeC != NULL)
		wayPos = closeC->getPosition();
	else
		wayPos = w->getPosition();
	tan = getTangent();
	toWaypoint = wayPos - rocketPos;		
	angleRot = toWaypoint -  tan.normalized();

	turningScalar = angleRot.dot(getBinormal());
	rateOfChange = lastAngleForce - turningScalar;

	roationForce = turningScalar*turningModifier - rateOfChange*rateOfChangeModifier;
	lastAngleForce = turningScalar;

	distance = toWaypoint.length();
	forwardForce = btScalar(-distance*forwardModifier);
	//btScalar forwardForce = btScalar(w->getThrottle());
	if(roationForce > maxMovementForce)
		roationForce = maxMovementForce;
	else if(roationForce < -maxMovementForce)
		roationForce = - maxMovementForce;

	btVector3 mov = getPosition() + toWaypoint * .1;	
	this->physicsObject->getWorldTransform().getOrigin().setValue(mov.x(), mov.y(), mov.z());


	//RotationEvent* re = new RotationEvent(btQuaternion(0, roationForce,0,0));
	////c->observeRotation(re);	
	//
	//delete re;
	//
	//if (forwardForce > maxMovementForce)
	//	forwardForce = maxMovementForce;
	//else if (forwardForce < -maxMovementForce)
	//	forwardForce = -maxMovementForce;
	//ForwardForceEvent* ffe = new ForwardForceEvent(forwardForce, forwardForce/32767.0f);
	////c->observeForwardForce(ffe);
	//delete ffe;
}


Car* Rocket::getClosestCar(bool inFront, btScalar &detectionRange)
{
	btScalar distance(detectionRange);
	int index = -1;
	btAlignedObjectArray<Car*>* cList = EntityManager::getInstance()->getCarList();
	for(int i = 0; i < cList->size(); i++)
	{		
		Car* c = cList->at(i);
		btVector3 directionVect = getPosition() - c->getPosition();
		btScalar tempDist(directionVect.length());
		if (inFront)
		{
			if (directionVect.dot(getTangent()) < 0.03)
				continue;
		}
		if (tempDist < distance)
		{
			distance = tempDist;
			index = i;
		}
	}
	if (index != -1)
		return cList->at(index);
	return NULL;
}

int Rocket::getNextWaypointIndex(){return nextWaypoint;}

void Rocket::setNextWaypointIndex(int index){nextWaypoint = index;}

void Rocket::positionCheck(Waypoint* w){
	btVector3 toWaypoint = w->getPosition() - getPosition();
	//btVector3 toDot = toWaypoint - car->getTangent().normalized();
	
	//If amount is less than 0, then car is past this.
	btScalar amount = w->getTangent().dot(toWaypoint);	

	if (amount < 25.0 && amount > 0){					
		if (!w->getWaypointList().empty())
			setNextWaypointIndex(w->getWaypointList().at(0)->getIndex());
	}
}

void Rocket::reloadVariables(ReloadEvent *e){
	turningModifier = e->numberHolder.aiInfo.rotateModifier;
	forwardModifier = e->numberHolder.aiInfo.drivingModifier;
	maxMovementForce = e->numberHolder.aiInfo.maxMovementForce;
	rateOfChangeModifier = e->numberHolder.aiInfo.rateOfChangeModifier;
}