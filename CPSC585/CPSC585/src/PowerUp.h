#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"

enum PowerUpType
{
	EMPTY = 0,
	SPEED,
	ROCKET,
	NOVA,
	SLOW,
	TRACTION,
	SHIELD
	//Landmine?
};

class PowerUp : public Entity
{
public:
	PowerUp();

	int GetType();
	void SetType( int type );
	void initObservers();
	bool initPhysicsObject( btCollisionShape* cShape, btScalar &mass, btTransform &trans );
	btCollisionObject* getPhysicsObject();
	bool isHit();
	void hitPowerup();
private:
	int m_Type;
	bool collected;
};

#endif