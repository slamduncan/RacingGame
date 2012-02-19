#include "Waypoint.h"

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