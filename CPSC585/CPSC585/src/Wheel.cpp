#include "Wheel.h"

Wheel::Wheel(btScalar radius, btScalar lengthOfSpring, btScalar lengthOfSpringWithCar,
		btScalar kVal, btScalar cVal, btVector3 &gravityIn, btVector3 &springAttachmentPoint,
		btVector3 &endOfSpringPoint, btRigidBody* physicsRigid)
{

			hoverValue = radius;
			restLengthOfSpring = lengthOfSpring;
			kValue = kVal;
			cValue = cVal;
			gravity = gravityIn;
			attachedToCarPosition = springAttachmentPoint;
			bottomSpringPosition = endOfSpringPoint;
			physicsObject = physicsRigid;
			dampingModifier = 1.0f;
			kModifier = 1.0f;
			dynamicsWorld = Physics::Inst()->getDiscreteDynamicsWorld();
}

btVector3 Wheel::getAttachedToCarPosition(){
	return attachedToCarPosition;
}

btVector3 Wheel::getBottomSpringPosition(){
	return bottomSpringPosition;
}

void Wheel::calcForce(btVector3 &springLocation, btVector3 &carNormal)
{
	attachedToCarPosition = springLocation;
	//Note: Should check to make sure the car isn't bottoming out
	// bottomSpringPosition <= bottom of chassie

	//Find the bottom of the current spring.

	/* This should cast a ray from the position that the spring is attached
	   down the direction of the spring(the car axis) for the length of the spring
	   plus the length of the hover value. */
	
	//Find the end point of the ray we are casting.
	btVector3 rayEnd = attachedToCarPosition - ((restLengthOfSpring + hoverValue) * carNormal);
	//Set up the length of the ray and direction for the cast.
	btCollisionWorld::ClosestRayResultCallback RayCallback(attachedToCarPosition, rayEnd);
	//Do the actual cast.
	dynamicsWorld->rayTest(attachedToCarPosition, rayEnd, RayCallback);

	bool inGround = false;
	bottomSpringPosition = attachedToCarPosition - restLengthOfSpring * carNormal;
	/*If we have hit something */
	if (RayCallback.hasHit())
	{
		//Distance into the ground the ray went. (Currently assuming the ground is normal)
		//CHANGE THIS CODE WHEN WE START ADDING BUMPS.
		btScalar disInGrd = btVector3(0,1,0).dot(RayCallback.m_hitPointWorld - rayEnd);
		
		
		//Inside of ground
		if (disInGrd >= 0.0f)
		{
			inGround = true;
			//Move the spring up so that the "wheel" or hover value is between it and the gound still
			bottomSpringPosition = RayCallback.m_hitPointWorld + hoverValue * carNormal;
		}
		//Else we are off the ground so no force???	

	}
	if (!inGround)
	{
		return;
	}

	//Assuming here that bottomSpringPosition is calculated correctly.
	//Check here to make sure displacement is correct.... May be reversed due to directions
	btScalar displacement = restLengthOfSpring  - ((attachedToCarPosition - bottomSpringPosition).length());
	
	// Car Normal is here to provide the direction of the force.
	btVector3 initialForce = (kValue * displacement * carNormal.normalized()) * kModifier ;

	
	//Get the speed of dsiplacement for the attacment point of the spring.
	btVector3 speedOfDisplacement = btVector3(0,0,0);
	speedOfDisplacement = physicsObject->getVelocityInLocalPoint(attachedToCarPosition);
	//Use this speed times crossed onto the wheel axis (carNormal) to find how much of that
	//Velocity is being applied along the wheel axis. This will affect the wheel
	// dampening.
	btVector3 suspensionVelocity = carNormal.normalized().dot(speedOfDisplacement) * carNormal.normalized();
	//Use that speed and the critical damping value to find the damping force.
	btVector3 dampingForce = 0 * -cValue * suspensionVelocity * dampingModifier;


	//Combine the forces to get the total force that should be applied.
	btVector3 totalForce = initialForce + dampingForce;

	printf("(%.3f, %.3f, %.3f),(%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f), \n", totalForce.x(), totalForce.y(), totalForce.z(), initialForce.getX(), initialForce.getY(), initialForce.getZ(), dampingForce.getX(), dampingForce.getY(), dampingForce.getX());

	if (totalForce.length() > 30)
		 totalForce *= 30 / totalForce.length();
	physicsObject->applyForce(totalForce, attachedToCarPosition);
}
