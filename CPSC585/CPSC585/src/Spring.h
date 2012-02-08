#ifndef SPRING_H
#define SPRING_H
#include "LinearMath/btVector3.h"
#include "btBulletDynamicsCommon.h"
#include "Physics.h"


class Spring
{
public:
	btVector3 springVector;
	
	Spring(void);
	Spring(btRigidBody* phys);
	~Spring(void);
	void update(btVector3 &springLocation, btVector3 &carNormal);


private:
	btScalar MAGICEQUILIBRIUMLENGTH;
	btScalar MAGICSPRINGCONSTANT;

	btRigidBody* physicsObject;
	btDiscreteDynamicsWorld* dynamicsWorld;

};
#endif