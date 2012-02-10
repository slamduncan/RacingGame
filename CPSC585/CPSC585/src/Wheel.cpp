#include "Wheel.h"

Wheel::Wheel(btScalar &radius, btScalar &lengthOfSpring, btScalar &lengthOfSpringWithCar,
		btScalar &kVal, btVector3 &gravityIn, btVector3 &springAttachmentPoint,
		btVector3 &endOfSpringPoint, btRigidBody* physicsRigid)
{

			radiusOfWheel = radius;
			totalLengthOfSpring = lengthOfSpring;
			kValue = kVal;
			gravity = gravityIn;
			attachedToCarPosition = springAttachmentPoint;
			bottomSpringPosition = endOfSpringPoint;
}



void Wheel::calcForce()
{




}
