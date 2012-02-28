#include "Waypoint.h"

Waypoint::Waypoint(){
	nextWaypoints = std::vector<Waypoint*>();
}

bool Waypoint::initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location){
	
	transform = location;

	return true;
}
void Waypoint::initObservers()
{

}

btScalar* Waypoint::getGLMatrix()
{
	transform.getOpenGLMatrix(glMatrix);

	return glMatrix;
}

btVector3 Waypoint::getPosition()
{
	return transform.getOrigin();
}
btVector3 Waypoint::getTangent()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 tan = orientation.getColumn(0);

	return tan;
}

btVector3 Waypoint::getNormal()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 nor = orientation.getColumn(1);

	return nor;
}
btVector3 Waypoint::getBinormal()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 bin = orientation.getColumn(2);

	return bin;
}

void Waypoint::positionCheck(Car* car){
	btVector3 toWaypoint = getPosition() - car->getPosition();
	//btVector3 toDot = toWaypoint - car->getTangent().normalized();
	
	//If amount is less than 0, then car is past this.
	btScalar amount = getTangent().dot(toWaypoint);

	if (amount < 0.0 && amount > -10.0){
		int nextIndex = nextWaypoints.at(0)->getIndex();
		//Do stuff to find the next waypoint!
		if (!nextWaypoints.empty())
			car->setNextWaypointIndex(nextIndex);
	}
}

void Waypoint::addNextWaypoint(Waypoint *waypoint){
	int size = nextWaypoints.size();
	for (int i = 0; i < size; i++)
	{
		if (nextWaypoints.at(i)->getIndex() == waypoint->getIndex())
		{			
			return;
		}
	}

	nextWaypoints.push_back(waypoint);
}

int Waypoint::getIndex(){
	return index;
}
void Waypoint::setIndex(int in){index = in;}

void Waypoint::removeWaypointFromList(int indexOfWaypointToRemove){
	int size = nextWaypoints.size();	
	for (int i = 0; i < size; i++)
	{
		if (nextWaypoints.at(i)->getIndex() == indexOfWaypointToRemove)
		{
			nextWaypoints.erase(nextWaypoints.begin() + i);
			return;
		}
	}
}