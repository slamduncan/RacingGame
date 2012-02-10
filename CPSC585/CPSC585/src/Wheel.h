#ifndef WHEEL_H
#define WHEEL_H

#include "LinearMath\btScalar.h"
#include "LinearMath\btVector3.h"
#include "btBulletDynamicsCommon.h"
#include "Physics.h"

class Wheel {
public:
	Wheel(){}
	Wheel(btScalar radius, btScalar lengthOfSpring, btScalar lengthOfSpringWithCar,
		btScalar kVal, btScalar cVal, btVector3 &gravityIn, btVector3 &springAttachmentPoint,
		btVector3 &endOfSpringPoint, btRigidBody* physicsRigid);

	void calcForce(btVector3 &springLocation, btVector3 &carNormal);

private:
	btScalar radiusOfWheel;
	btScalar restLengthOfSpring;
	btScalar lengthOfSpringWithCar;
	btScalar kValue, cValue;
	btVector3 gravity;
	btVector3 attachedToCarPosition;
	btVector3 bottomSpringPosition;
	btRigidBody* physicsObject;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btScalar dampingModifier, kModifier, hoverValue;
};
#endif