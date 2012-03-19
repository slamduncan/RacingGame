#ifndef SPAWNABLE_H
#define SPAWNABLE_H
#include "Entity.h"

class Spawnable : public Entity
{
public:
	Spawnable();
	~Spawnable();

	void initObservers();
	bool initPhysicsObject( btCollisionShape* cShape, btScalar &mass, btTransform &trans );
};

#endif