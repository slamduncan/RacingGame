#include "AIHandler.h"

AIHandler* AIHandler::instance = 0;

AIHandler::AIHandler(){
	waypoints = EntityManager::getInstance()->getWaypointList();
	cars = EntityManager::getInstance()->getCarList();
}

void AIHandler::generateNextMove(){
	for(int i = 1; i < cars->size(); i++){
		Car* c = cars->at(i);
		int waypointIndex = c->getNextWaypointIndex();
		Waypoint* w = waypoints->at(waypointIndex);

		btVector3 carPos =  c->getPosition();
		btVector3 wayPos = w->getPosition();
		btScalar angle = carPos.angle(wayPos);
		c->observeRotation(new RotationEvent(btQuaternion(c->getNormal(), -angle)));
		int p = 0;
	}
}