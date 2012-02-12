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
			dampingModifier = .001f;
			kModifier = .01f;
			//kModifier = 0.5f;
			//

			onGround = false;

			dynamicsWorld = Physics::Inst()->getDiscreteDynamicsWorld();

			hitObject = NULL;

}

Wheel::Wheel(WheelInfo &wi, btVector3 &gravity, btRigidBody* physicsObject)
{

}

btVector3 Wheel::getAttachedToCarPosition(){
	return attachedToCarPosition;
}

btVector3 Wheel::getBottomSpringPosition(){
	return bottomSpringPosition;
}

btVector3 Wheel::calcForce(btVector3 &springLocation, btVector3 &carNormal)
{
	//ORIGINAL STUFF.
	attachedToCarPosition = springLocation;
	//Note: Should check to make sure the car isn't bottoming out
	// bottomSpringPosition <= bottom of chassie

	//Find the bottom of the current spring.

	// This should cast a ray from the position that the spring is attached
	//   down the direction of the spring(the car axis) for the length of the spring
	//   plus the length of the hover value. 

	//Find the end point of the ray we are casting.
	btVector3 rayEnd = attachedToCarPosition - ((restLengthOfSpring + hoverValue) * carNormal);
	//Set up the length of the ray and direction for the cast.
	btCollisionWorld::ClosestRayResultCallback RayCallback(attachedToCarPosition, rayEnd);
	//Do the actual cast.
	dynamicsWorld->rayTest(attachedToCarPosition, rayEnd, RayCallback);

	bool inGround = false;
	bottomSpringPosition = attachedToCarPosition - restLengthOfSpring * carNormal;
	//If we have hit something 
	if (RayCallback.hasHit())
	{
		onGround = true;

		hitObject = RayCallback.m_collisionObject;
		
		//Distance into the ground the ray went. (Currently assuming the ground is normal)
		//CHANGE THIS CODE WHEN WE START ADDING BUMPS.

		//btScalar disInGrd = btVector3(0,1,0).dot(RayCallback.m_hitPointWorld - rayEnd);
		btScalar disInGrd = (RayCallback.m_hitPointWorld - rayEnd).length();

		//Inside of ground
		if (disInGrd >= 0.0f)
		{
			inGround = true;
			//Move the spring up so that the "wheel" or hover value is between it and the gound still
			//bottomSpringPosition = RayCallback.m_hitPointWorld + hoverValue * carNormal;

			bottomSpringPosition = RayCallback.m_hitPointWorld;
		}				
	}
	//Else we are off the ground so no force???	
	if (!inGround)
	{		
		return btVector3(0,0,0);
	}

	btVector3 FORCEDIRECTION = carNormal;
	//btVector3 FORCEDIRECTION = btVector3(0,1,0);

	//Assuming here that bottomSpringPosition is calculated correctly.
	//Check here to make sure displacement is correct.... May be reversed due to directions
	btScalar displacement = restLengthOfSpring  - ((attachedToCarPosition - bottomSpringPosition).length());

	// Car Normal is here to provide the direction of the force.
	btVector3 initialForce = (kValue * displacement * FORCEDIRECTION) * kModifier;

	//Get the speed of dsiplacement for the attacment point of the spring.
	btVector3 speedOfDisplacement = btVector3(0,0,0);
	speedOfDisplacement = physicsObject->getVelocityInLocalPoint(RayCallback.m_hitPointWorld - physicsObject->getCenterOfMassPosition());

	//printf("Speed of Displacement: %.3f, %.3f, %.3f\n",speedOfDisplacement.getX(),speedOfDisplacement.getY(),speedOfDisplacement.getZ());

	//Use this speed times crossed onto the wheel axis (carNormal) to find how much of that
	//Velocity is being applied along the wheel axis. This will affect the wheel
	// dampening.
	btScalar suspensionVelocity = FORCEDIRECTION.dot(speedOfDisplacement);

	//printf("Suspension Velocity: %.3f\n",suspensionVelocity);

	//Use that speed and the critical damping value to find the damping force.
	btScalar dampingForce = cValue * suspensionVelocity * dampingModifier;


	//Combine the forces to get the total force that should be applied.
	btVector3 totalForce = initialForce - dampingForce*FORCEDIRECTION;

	//printf("(%.3f, %.3f, %.3f),(%.3f, %.3f, %.3f), %.3f \n", totalForce.x(), totalForce.y(), totalForce.z(), initialForce.getX(), initialForce.getY(), initialForce.getZ(), dampingForce);

	btScalar maxForce = 30.0f / physicsObject->getInvMass();

	if (totalForce.length() > maxForce)
		totalForce = totalForce.normalize() * maxForce;
	//physicsObject->applyImpulse(totalForce, attachedToCarPosition);

	return totalForce;
}
