#include "AIHandler.h"

AIHandler* AIHandler::instance = 0;

AIHandler::AIHandler() : reloadObserver(this, &AIHandler::reloadVariables){
	waypoints = EntityManager::getInstance()->getWaypointList();
	cars = EntityManager::getInstance()->getCarList();
	turningModifier = 1.0;
}

void AIHandler::generateNextMove(){
	for(int i = 1; i < cars->size(); i++){
		Car* c = cars->at(i);
		int waypointIndex = c->getNextWaypointIndex();
		Waypoint* w = waypoints->at(waypointIndex);

		btVector3 carPos =  c->getPosition();
		btVector3 wayPos = w->getPosition();
		btVector3 tan = c->getTangent();
		btVector3 toWaypoint = wayPos - carPos;		
		btVector3 angleRot = toWaypoint -  tan.normalized();
		btScalar k = angleRot.dot(c->getBinormal());

		c->observeRotation(new RotationEvent(btQuaternion(0, k*turningModifier,0,0)));		
		int p = 0;
	}
}

void AIHandler::reloadVariables(ReloadEvent *e){
	turningModifier = e->numberHolder.aiInfo.rotateModifier;
}