#pragma once
#include "Spawnable.h"
#include "Car.h"
#include "Time.h"

class SlowField:public Spawnable
{
public:
	//btGhostObject * slowBlobContainer;
	btCompoundShape* blobContainer;
	SlowField(Car* car);
	~SlowField(void);
	clock_t timeToSelfDestruct;
	clock_t timeToDrop;
	clock_t timeToDelete;
	int numSpawned, numDeleted;

	Car* getCar();

	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &tran);

private:
	Car * car;
};
