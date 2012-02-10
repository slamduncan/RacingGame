#ifndef WHEEL_H
#define WHEEL_H

#include "LinearMath\btScalar.h"
#include "LinearMath\btVector3.h"
#include "btBulletDynamicsCommon.h"
#include "Physics.h"

class Wheel {
public:
	Wheel(btScalar &radius, btScalar &lengthOfSpring, btScalar &lengthOfSpringWithCar,
		btScalar &kVal, btVector3 &gravityIn, btVector3 &springAttachmentPoint,
		btVector3 &endOfSpringPoint, btRigidBody* physicsRigid);

	void calcForce();

private:
	btScalar radiusOfWheel;
	btScalar totalLengthOfSpring;
	btScalar lengthOfSpringWithCar;
	btScalar kValue;
	btVector3 gravity;
	btVector3 attachedToCarPosition;
	btVector3 bottomSpringPosition;
	btRigidBody* physicsObject;
	btDiscreteDynamicsWorld* dynamicsWorld;
};
#endif