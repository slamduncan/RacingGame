#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Entity.h"

class Waypoint : public Entity {

public:
	bool initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location);
	void initObservers();
};

#endif