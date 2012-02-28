#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Entity.h"
#include "Car.h"

class Waypoint : public Entity {
	btTransform transform;
	std::vector<Waypoint*> nextWaypoints;
	int index;
public:
	
	bool initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location);
	void initObservers();

	btScalar* getGLMatrix();
	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();

	Waypoint();

	void positionCheck(Car* car);
	void addNextWaypoint(Waypoint* waypoint);
	int getIndex();
	void setIndex(int in);
	void removeWaypointFromList(int indexOfWaypointToRemove);
	std::vector<Waypoint*> getWaypointList();
	std::string toString();
};

#endif