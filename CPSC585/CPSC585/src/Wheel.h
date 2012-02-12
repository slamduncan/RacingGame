#ifndef WHEEL_H
#define WHEEL_H

#include "LinearMath\btScalar.h"
#include "LinearMath\btVector3.h"
#include "btBulletDynamicsCommon.h"
#include "Physics.h"

struct WheelInfo
{
	btScalar w_wheelRadius;
	btScalar w_relaxLength;	// length of the spring without any forces on it
	btScalar w_restDisplacement;	// may or may not need this
	btScalar w_maxDisplacement;	// maximum amount a spring can compress or stretch
	btScalar w_k_suspension;	// spring stiffness
	btScalar w_k_suspensionScale;	// scale stiffness
	btScalar w_k_damping;	
	btScalar w_k_dampingScale;
	btScalar w_k_compression;
	btVector3 w_attachPointLS;	// spring attachment point in local space
	btVector3 w_carNormalLS;	// car normal in local space probably don't need since the physicsObject has this
};

class Wheel {
public:
	bool onGround;
	
	
	Wheel(){}
	Wheel(btScalar radius, btScalar lengthOfSpring, btScalar lengthOfSpringWithCar,
		btScalar kVal, btScalar cVal, btVector3 &gravityIn, btVector3 &springAttachmentPoint,
		btVector3 &endOfSpringPoint, btRigidBody* physicsRigid);

	Wheel(WheelInfo &wi, btVector3 &gravity, btRigidBody* physicsObject);

	void setKModifier(btScalar &inK);
	void setCModifier(btScalar &inC);


	btVector3 calcForce(btVector3 &springLocation, btVector3 &carNormal);

	btVector3 getAttachedToCarPosition();
	btVector3 getBottomSpringPosition();

	btVector3 impulse;
	btScalar springLength;
	btVector3 attachPointWS;
	btVector3 endPointWS;

	btCollisionObject* hitObject;

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