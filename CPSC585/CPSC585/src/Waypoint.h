#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Entity.h"

class Waypoint : public Entity {
	btTransform transform;
public:
	
	bool initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location);
	void initObservers();

	btScalar* getGLMatrix();
	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();
};

#endif