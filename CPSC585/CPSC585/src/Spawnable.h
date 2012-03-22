#ifndef SPAWNABLE_H
#define SPAWNABLE_H
#include "Entity.h"
#include "Time.h"

class Spawnable : public Entity
{
public:
	Spawnable();
	~Spawnable();

	void initObservers();
	void setSelfDestructTime(int secondsFromNow);
	bool initPhysicsObject( btCollisionShape* cShape, btScalar &mass, btTransform &trans );

	clock_t timeToSelfDestruct;
};

#endif