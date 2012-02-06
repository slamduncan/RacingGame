#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Entity.h"

class Waypoint : public Entity {

public:
	bool initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location){
		btVector3 inertia = btVector3(0,0,0);
		
		//shape->calculateLocalInertia(mass, inertia);
				
		btDefaultMotionState* entMotionState = new btDefaultMotionState(location);
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,shape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);
		physicsObject->setCollisionFlags(btRigidBody::CF_NO_CONTACT_RESPONSE);

		return true;};
};

#endif