#ifndef EFFECT_H
#define EFFECT_H

#include "Entity.h"
#include "Timer.h"

enum EFFECTTYPE
{
	SPEED = 0,
	NOVA,
};

class Effect : public Entity
{
private:
	btTransform transform;
	btScalar ttl;
	Timer timer;
	Entity* spawner;
	int type;

public:
	btScalar scale;

	Effect();
	Effect(btScalar ttl, btTransform& trans, int type);
	~Effect();

	void startTimer();
	void stopTimer();
	
	void initObservers();
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	btScalar* getGLMatrix();
	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();

	bool timeUp();
	int getType();
};

#endif
