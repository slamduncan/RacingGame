#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"

enum PowerUpType
{
	SPEED = 0,
	PROJECTILE,
	TRACTION,
	EMPTY
};

class PowerUp : public Entity
{
public:
	PowerUp();

	int GetType();
	void SetType( int type );
	void initObservers();
	bool initPhysicsObject( btCollisionShape* cShape, btScalar &mass, btTransform &trans );
private:
	int m_Type;
};

#endif