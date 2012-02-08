#include "Track.h"

Track::Track()
{

}

Track::~Track()
{

}

/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Track::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	//objShape = new btBvhTriangleMeshShape(bttm, true, true);
	
	// there is a collision shape
	if(cShape != NULL)
	{
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		// entRigidBodyCI(mass, motion state, collision shape, inertia);
		// need to update the mass to make it as a variable?
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,cShape,btVector3(0, 0, 0));

		physicsObject = new btRigidBody(entRigidBodyCI);

		return true;
	}
	return false;
}

void Track::initObservers()
{

}