#pragma once
#include "Spawnable.h"
#include "Car.h"

class Mine:public Spawnable
{
public:
	Mine(Car* c);
	~Mine(void);
	bool initPhysicsObject(btCollisionShape* cShape, btTransform &tran);
};
