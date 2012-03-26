#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"
#include "Time.h"

enum PowerUpType
{
	EMPTY = 0,
	SPEED_SLOW,
	ROCKET_SHIELD,
	NOVA_MINE,		
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
	bool isCollected();
	void setCollected(bool is);
	clock_t timeToRespawn;
private:
	int m_Type;
	bool collected;
};

#endif