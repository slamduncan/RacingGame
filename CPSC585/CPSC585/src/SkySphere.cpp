#include "SkySphere.h"

SkySphere::SkySphere()
{
}

SkySphere::~SkySphere()
{
}

void SkySphere::initObservers()
{

}
bool SkySphere::initPhysicsObject(btCollisionShape * cShape, btScalar &mass, btTransform &trans)
{
	transform = trans;
	return true;
}


btVector3 SkySphere::getTangent()
{
	return transform.getBasis().getColumn(0);
}

btVector3 SkySphere::getNormal()
{
	return transform.getBasis().getColumn(1);
}

btVector3 SkySphere::getBinormal()
{
	return transform.getBasis().getColumn(2);
}

btTransform SkySphere::getTransform()
{
	return transform;
}

void SkySphere::setTransform(btTransform &inT)
{
	transform = inT;
}
btScalar* SkySphere::getGLMatrix()
{
	transform.getOpenGLMatrix(glMatrix);

	return glMatrix;
}