#include "Waypoint.h"

Waypoint::Waypoint() : updateVariableObserver(this,&Waypoint::observeVariables){
	nextWaypoints = std::vector<Waypoint*>();
	goToNextWaypointDistanceBefore = 0.0;
	goToNextWaypointDistanceAfter = -10.0;
	throttlePercentage = 1.0;
	throttleValue = -32767.0;
	split = false;
	converge = false;
	direction = 0;
}

int Waypoint::getThrottle(){return throttleValue;}

void Waypoint::setThrottle(int value){
	throttleValue = value;
}

bool Waypoint::initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location){
	
	transform = location;

	return true;
}
void Waypoint::initObservers()
{
	updateVariableObserver.init(EventTypes::RELOAD_VARIABLES);
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
	btScalar amount2 = toWaypoint.dot(getTangent());	

	if (amount < goToNextWaypointDistanceBefore && amount > goToNextWaypointDistanceAfter){
		int id = car->id;
		int nextIndex = 0;
		printf("%d\n", direction);
		if (!nextWaypoints.empty()){			
			if (direction % nextWaypoints.size() == 0)
			{
				nextIndex = nextWaypoints.at(0)->getIndex();
				printf("%d\n", nextIndex);
				direction = id % nextWaypoints.size();
			}
			else
			{
				nextIndex = nextWaypoints.at(1)->getIndex();
				printf("%d\n", nextIndex);
				direction = id % nextWaypoints.size();
			}
				//Do stuff to find the next waypoint!
				if (!nextWaypoints.empty())
					car->setNextWaypointIndex(nextIndex);
		}

	}
	//Else look ahead for one closer?
	//int closerPointIndex = -1;
	//btScalar turnFactor = (toWaypoint - car->getTangent()).dot(car->getBinormal());
	//btScalar length = (getPosition() - car->getPosition()).length();
	//Waypoint* possibleWayPoint = nextWaypoints.at(0);
	//for (int i = 0; i < 2; i++)
	//{
	//	toWaypoint = possibleWayPoint->getPosition() - car->getPosition();
	//	btScalar tempTurnFactor = (toWaypoint - car->getTangent()).dot(car->getBinormal());
	//	btScalar tempLength = toWaypoint.length();
	//	amount = possibleWayPoint->getTangent().dot(toWaypoint);
	//	if (abs(tempTurnFactor) < abs(turnFactor) && amount > 0)
	//	{
	//		turnFactor = tempTurnFactor;
	//		length = tempLength;
	//		closerPointIndex = possibleWayPoint->getIndex();
	//	}
	//	possibleWayPoint = possibleWayPoint->getWaypointList().at(0);
	//}
	//if (closerPointIndex != -1)
	//	car->setNextWaypointIndex(closerPointIndex);
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
	if (nextWaypoints.size() >= 2)
		split = true;
	else
		split = false;
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
			if (nextWaypoints.size() <= 1)
				split = false;
			else
				split = true;
			return;
		}
	}
}

std::vector<Waypoint*> Waypoint::getWaypointList(){return nextWaypoints;}

std::string Waypoint::toString()
{
	std::stringstream stream;
	stream << getPosition().x() << " " << getPosition().y() << " " << getPosition().z() << " ";
	for (int i = 0; i < 3; i++)
	{
		btVector3 row = transform.getBasis().getRow(i);
		stream << row.x() << " " << row.y() << " " << row.z() << " ";
	}
	stream << getThrottle() << " ";
	stream << "\n";
	if (nextWaypoints.size() >= 2)
	{
		split = true;
		stream << "SPLIT\n" << nextWaypoints.size() << " \n";
	}
	if (converge)
		stream << "CONVERGE\n";
	
	return stream.str();
}

void Waypoint::observeVariables(ReloadEvent *e){
	goToNextWaypointDistanceBefore = e->numberHolder.aiInfo.goToNextWaypointDistanceBefore;
	goToNextWaypointDistanceAfter = e->numberHolder.aiInfo.goToNextWaypointDistanceAfter;
}

void Waypoint::setTransform(btTransform &inT){
	transform = inT;
}

btTransform Waypoint::getTransform(){return transform;}