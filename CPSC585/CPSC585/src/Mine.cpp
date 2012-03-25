#include "Mine.h"

Mine::Mine(Car* car)
{
}

Mine::~Mine(void)
{
}

bool Mine::initPhysicsObject(btCollisionShape* cShape, btTransform &trans)
{
	if(cShape != NULL)
	{

		physicsObject = new btGhostObject();

		physicsObject->setCollisionShape(cShape);
		physicsObject->setWorldTransform(trans);

		physicsObject->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);
		return true;
	}
	
	return false;
}