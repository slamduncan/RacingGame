#include "AIHandler.h"

AIHandler* AIHandler::instance = 0;

AIHandler::AIHandler() : reloadObserver(this, &AIHandler::reloadVariables){
	waypoints = EntityManager::getInstance()->getWaypointList();
	cars = EntityManager::getInstance()->getCarList();
	turningModifier = 1.0;
	forwardModifier = 1.0;
	maxMovementForce = 1.0;
	rubberBandModifier = 1.0;
	rammingRange = 10.0;	
	reloadObserver.init(EventTypes::RELOAD_VARIABLES);
}

void AIHandler::generateNextMove(){
	Car* humanCar = cars->at(0);
	for(int i = 1; i < cars->size(); i++){
		Car* c = cars->at(i);
		Car* closeC = c->getClosestCar(true, rammingRange);

		btScalar forwardForce, turningScalar, rateOfChange, roationForce, distance;
		btVector3 carPos, wayPos, tan, toWaypoint, angleRot;

		int waypointIndex = c->getNextWaypointIndex();
		if (waypointIndex == -1){
			return;
		}
		Waypoint* w = waypoints->at(waypointIndex);
		w->positionCheck(c);
		bool changed = false;
		while (waypointIndex != c->getNextWaypointIndex())
		{			
			Waypoint* w = waypoints->at(waypointIndex);
			w->positionCheck(c);
			waypointIndex = c->getNextWaypointIndex();
			changed = true;			
		}

		if( i == 0 )
			continue;
		if (!changed)			
			c->AIresetCounter++;
		else 
			c->AIresetCounter = 0;

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
		/*rubber banding effect */
		int waypointDiff = 0;
		if (abs(humanCar->getNextWaypointIndex() - c->getNextWaypointIndex()) < 650)
			waypointDiff = humanCar->getNextWaypointIndex() - c->getNextWaypointIndex();
		if (c->lapCount - humanCar->lapCount != 0)
			waypointDiff += EntityManager::getInstance()->numWaypoints() * (humanCar->lapCount - c->lapCount);
		//int waypointDiff = c->getPosition().setY(0.f) - humanCar->getPosition().setY(0.f)
		forwardForce = forwardForce - waypointDiff * rubberBandModifier;

		//User too good, warp forward!
		if (waypointDiff > 300 && (c->lapCount < humanCar->lapCount || c->halfWayAround != humanCar->halfWayAround))   //abs(forwardForce) > (35000.0f + 400 * rubberBandModifier) && forwardForce < 0 && c->lapCount <= humanCar->lapCount)
		{
			EntityManager* entM = EntityManager::getInstance();
			Waypoint* moveCarTo = entM->getWaypoint(((humanCar->getNextWaypointIndex() - 10) + entM->numWaypoints()) % entM->numWaypoints());
			int previousIndex = c->getNextWaypointIndex();
			c->physicsObject->setWorldTransform(moveCarTo->getTransform());
			if (moveCarTo->getIndex() < entM->numWaypoints()/2)
			{
				c->halfWayAround = false;
			}
			else
			{
				c->halfWayAround = true;
			}

			if (previousIndex > moveCarTo->getIndex())
			{
				c->lapCount++;
			}
//			c->lapCount = humanCar->lapCount;
//			c->halfWayAround = humanCar->halfWayAround;
			c->setNextWaypointIndex(moveCarTo->getWaypointList().at(0)->getIndex());
			c->AIresetCounter = 0;
		}
		//User is bad, warp back :(
		else if (waypointDiff < -150 && c->lapCount >= humanCar->lapCount)
		{
			EntityManager* entM = EntityManager::getInstance();
			Waypoint* moveCarTo = entM->getWaypoint(((humanCar->getNextWaypointIndex() + 20) + entM->numWaypoints()) % entM->numWaypoints());
			c->physicsObject->setWorldTransform(moveCarTo->getTransform());			
			c->halfWayAround = humanCar->halfWayAround;
			c->setNextWaypointIndex(moveCarTo->getWaypointList().at(0)->getIndex());
			c->AIresetCounter = 0;

		}
		else
		{
			ForwardForceEvent* ffe = new ForwardForceEvent(forwardForce, forwardForce/32767.0f);
			c->observeForwardForce(ffe);
			delete ffe;
		}

		PowerUpCheck(c);
		//printf("Car: %d, Forward = %f, Turn = %f\n", i, forwardForce, roationForce);
	}
}

void AIHandler::PowerUpCheck(Car* c)
{
	if (c->usedPowerUpRecently)
		return;
	for (int i= 0; i < 3; i++)
	{
		PowerUp* p = c->GetPowerUpAt(i);	
		Car* closestFrontCar = c->getClosestCar(true);
		Car* closestBackCar = c->getClosestCar(false);
		bool carInFront = closestFrontCar != NULL;
		bool carBehind = closestBackCar != NULL && carInFront == false;
		switch(p->GetType())
		{
		case(ROCKET_SHIELD):
			{
				//Rocket
				if (carInFront)
				{
					if ((closestFrontCar->getPosition() - c->getPosition()).length() > 15.0)
						c->UsePowerUp(i, true);
				}
				else 
				{
					//Shield
					btAlignedObjectArray<Spawnable*>* spawnList = EntityManager::getInstance()->getSpawnableList();
					for (int j = 0; j < spawnList->size(); j++)
					{
						if ((spawnList->at(j)->getPosition() - c->getPosition()).length() < 20.0 && spawnList->at(j)->carId != c->id)
							c->UsePowerUp(i, false);
					}
				}
				break;
			}
		case(SPEED_SLOW):
			{
				//Slow
				if (carBehind || c->GetNumberPowerUps() == 3)
				{
					c->UsePowerUp(i, true);
				}
				break;
			}
		case(NOVA_MINE):
			{
				//Nova
				if ((carInFront || carBehind) && ((closestBackCar->getPosition() - c->getPosition()).length() < 20.0))
				{
					c->UsePowerUp(i, false);
				}
				else if (carInFront || carBehind || c->GetNumberPowerUps() == 3)
					c->UsePowerUp(i, true);
				break;
			}
		}
		
	}
}

void AIHandler::reloadVariables(ReloadEvent *e){
	turningModifier = e->numberHolder.aiInfo.rotateModifier;
	forwardModifier = e->numberHolder.aiInfo.drivingModifier;
	maxMovementForce = e->numberHolder.aiInfo.maxMovementForce;
	rateOfChangeModifier = e->numberHolder.aiInfo.rateOfChangeModifier;
	rubberBandModifier = e->numberHolder.aiInfo.rubberBandModifier;
	rammingRange = e->numberHolder.aiInfo.rammingRange;
}