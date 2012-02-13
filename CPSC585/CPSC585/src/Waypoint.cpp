#include "Waypoint.h"

bool Waypoint::initPhysicsObject(btCollisionShape *shape, btScalar &mass, btTransform &location){
	
	transform = location;


	/*
	btVector3 inertia = btVector3(0,0,0);
	
	//shape->calculateLocalInertia(mass, inertia);
			
	btDefaultMotionState* entMotionState = new btDefaultMotionState(location);
	btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,shape,inertia);

	physicsObject = new btRigidBody(entRigidBodyCI);
	physicsObject->setCollisionFlags(btRigidBody::CF_NO_CONTACT_RESPONSE);
	*/
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