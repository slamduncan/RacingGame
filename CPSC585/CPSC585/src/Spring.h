#ifndef SPRING_H
#define SPRING_H
#include "LinearMath/btVector3.h"
#include "btBulletDynamicsCommon.h"
#include "Physics.h"


class Spring
{
public:
	btScalar springLength;
	
	Spring(void);
	Spring(btRigidBody* phys, btScalar kVal);
	~Spring(void);
	void update(btVector3 &springLocation, btVector3 &carNormal);


private:
	btScalar MAGICEQUILIBRIUMLENGTH;
	btScalar MAGICSPRINGCONSTANT;

	btVector3 force;

	btRigidBody* physicsObject;
	btDiscreteDynamicsWorld* dynamicsWorld;

};
#endif