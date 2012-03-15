#include "PowerUp.h"

PowerUp::PowerUp()
{
	m_Type = EMPTY;
	collected = false;
}

int PowerUp::GetType()
{
	return m_Type;
}

void PowerUp::SetType(int type)
{
	m_Type = type;
}

void PowerUp::initObservers()
{

}

bool PowerUp::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	if(cShape != NULL)
	{
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		// entRigidBodyCI(mass, motion state, collision shape, inertia);
		// need to update the mass to make it as a variable?
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,cShape,btVector3(0, 0, 0));

		physicsObject = new btGhostObject();

		physicsObject->setCollisionShape(cShape);
		physicsObject->setWorldTransform(trans);
		
		//printf("I have internal type value: %i\n",physicsObject->getInternalType());
		//physicsObject->seta
		physicsObject->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);
		return true;
	}
	return false;
}

void PowerUp::hitPowerup(){
	collected = true;
}

bool PowerUp::isHit(){
	return collected;
}