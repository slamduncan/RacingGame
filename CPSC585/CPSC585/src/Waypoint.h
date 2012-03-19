#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Entity.h"
#include "Car.h"
//#include "Rocket.h"

class Waypoint : public Entity {
	btTransform transform;
	std::vector<Waypoint*> nextWaypoints;
	int index;
	float goToNextWaypointDistanceBefore, goToNextWaypointDistanceAfter;
	MethodObserver<ReloadEvent, Waypoint> updateVariableObserver;
	float throttlePercentage;
	int throttleValue;
	int direction;
	
public:
	bool split;
	bool converge;
	bool initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location);
	void initObservers();

	btScalar* getGLMatrix();
	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();

	Waypoint();		

	void observeVariables(ReloadEvent *e);
	void positionCheck(Car* car);
	//void positionCheckRocket(Rocket* rocket);
	void addNextWaypoint(Waypoint* waypoint);
	int getIndex();
	void setIndex(int in);
	int getThrottle();
	void setThrottle(int value);
	void setTransform(btTransform &inT);
	btTransform getTransform();
	void removeWaypointFromList(int indexOfWaypointToRemove);
	std::vector<Waypoint*> getWaypointList();
	std::string toString();
};

#endif