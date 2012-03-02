#include "PowerUp.h"

PowerUp::PowerUp()
{
	m_Type = EMPTY;
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

		physicsObject = new btRigidBody(entRigidBodyCI);

		return true;
	}
	return false;
}