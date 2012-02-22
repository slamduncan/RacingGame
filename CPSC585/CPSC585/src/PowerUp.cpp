#include "PowerUp.h"

PowerUp::PowerUp()
{
	m_Type = EMPTY;
}

int PowerUp::GetType()
{
	return m_Type;
}

void PowerUp::SetType( int type )
{
	m_Type = type;
}

void PowerUp::initObservers()
{

}

bool PowerUp::initPhysicsObject( btCollisionShape* /*cShape*/, btScalar &/*mass*/, btTransform &/*trans*/ )
{
	return false;
}