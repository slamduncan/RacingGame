#include "Car.h"
#include "EventSystemHandler.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

//
//	Current Car representation
//
//                        ^ z
//                        |
//                        |
//            |-----------+------------|
//     wheel3 |           |            | wheel 2
//            |           |            |
//            |           |            |
//            |           |            |
//            |           |            |
//         x  |           |            |
//        <---+-----------.(y out of   |
//            |             screen)    |
//            |                        |
//            |                        |
//            |                        |
//            |                        |
//     wheel1 |                        | wheel 0
//            |------------------------|
Car::Car() : rotationObserver(this, &Car::observeRotation), 
			forwardForceObserver(this, &Car::observeForwardForce),
			updateVariableObserver(this, &Car::observeVariables)
{		
	Car::width = btScalar(5.0f);
	Car::height = btScalar(5.0f);
	Car::length = btScalar(10.0f);
	gravity = Physics::Inst()->getGravity();
	restDisplacement = btScalar(2.0f);
}

void Car::initObservers()
{	
	rotationObserver.init(EventTypes::ROTATION);
	forwardForceObserver.init(EventTypes::FORWARD_FORCE);

}

void Car::observeRotation(RotationEvent *e){		
	btVector3 test = e->getQuaternion().getAxis();
	btVector3 temp = physicsObject->getAngularVelocity();
	if (temp.length() < 5)
	//physicsObject->setAngularVelocity(test);
		physicsObject->applyTorque(test);		
}

void Car::observeForwardForce(ForwardForceEvent *e){
	
	btScalar engineForce = e->getNormForce();
	
	btVector3 tan = getTangent() * engineForce;
	tan.setY(0);	// project to the xz plane
	tan /= 4.0f;
	if(engineForce < 0)
	{
		// player is accelerating, we apply rear wheel force
		if(newWheels[2].onGround || newWheels[3].onGround)
		{
			physicsObject->applyImpulse(tan, wheelOffsets[2]);
			physicsObject->applyImpulse(tan, wheelOffsets[3]);
		}
	}
	// player is decelerating
	else
	{
		// apply to all the wheels
		for(int i = 0; i < 4; i++)
		{
			if(newWheels[i].onGround)
			{
				physicsObject->applyImpulse(tan, wheelOffsets[i]);
			}
		}
	}
}

/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Car::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	kVal = (mass * 10.0f)/(3.0f * 4.0f);
	if(cShape != NULL)
	{
		btVector3 inertia;
		
		cShape->calculateLocalInertia(mass, inertia);
		
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,cShape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);

		physicsObject->setActivationState(DISABLE_DEACTIVATION);

		updateSpringLocations();
		setUpWheelStuff();
		btScalar wheelLength(4.0f);

		/*
		printf("Wheel Offset 1: (%f, %f, %f)\n", wheelOffsets[0].getX(),wheelOffsets[0].getY(),wheelOffsets[0].getZ());
		printf("Wheel Offset 2: (%f, %f, %f)\n", wheelOffsets[1].getX(),wheelOffsets[1].getY(),wheelOffsets[1].getZ());
		printf("Wheel Offset 3: (%f, %f, %f)\n", wheelOffsets[2].getX(),wheelOffsets[2].getY(),wheelOffsets[2].getZ());
		printf("Wheel Offset 4: (%f, %f, %f)\n", wheelOffsets[3].getX(),wheelOffsets[3].getY(),wheelOffsets[3].getZ());
		*/

		//Bug found here; was using wheelOffsets[3] for newWheels 1,2 and 3.
		newWheels[0] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue, (gravity),(getPosition() + wheelOffsets[0]),
			(getPosition() + wheelOffsets[0] - getNormal()*3.0f), physicsObject);
		newWheels[1] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue, gravity, getPosition() + wheelOffsets[1],
			getPosition() + wheelOffsets[1] - getNormal()*3.0f, physicsObject);
		newWheels[2] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue,critDampingValue, gravity, getPosition() + wheelOffsets[2],
			getPosition() + wheelOffsets[2] - getNormal()*3.0f, physicsObject);
		newWheels[3] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue,gravity, getPosition() + wheelOffsets[3],
			getPosition() + wheelOffsets[3] - getNormal()*3.0f, physicsObject);			

		return true;
	}

	return false;
}

void Car::updateWheels()
{
	updateSpringLocations();

	btVector3 forces[4];
	btScalar sideFriction[4] = {btScalar(0.f),btScalar(0.f),btScalar(0.f),btScalar(0.f)}; 
	for (int i = 0; i < 4; i++){
		forces[i] = newWheels[i].calcForce(getPosition() + wheelOffsets[i], getNormal());
	}

	for(int i = 0; i < 4; i++)
	{
		
		if(newWheels[i].hitObject)
		{
			btVector3 contact = newWheels[i].getBottomSpringPosition();
			
			btRigidBody* groundObject = (class btRigidBody*) newWheels[i].hitObject;
			
			resolveSingleBilateral(*physicsObject, contact, *groundObject, contact, btScalar(0.),getBinormal(), sideFriction[i], 1/60.0f);
		}
	}

	

	for (int i = 0; i < 4; i++){
		btVector3 contact = newWheels[i].getBottomSpringPosition();
		physicsObject->applyImpulse(forces[i],contact - physicsObject->getCenterOfMassPosition()/*wheelOffsets[i]*/);
		
		if(sideFriction[i] != btScalar(1.))
		{
			btVector3 carNormal = getNormal();
			
			btVector3 relpos = contact - physicsObject->getCenterOfMassPosition();

			relpos -= carNormal * (carNormal.dot(relpos));

			physicsObject->applyImpulse(getBinormal() * sideFriction[i]*0.1f,relpos);
		}
		//cheatAndFixRotation();
	}
}

void Car::updateSpringLocations()
{
	btVector3 position = getPosition();
	btVector3 tangent = getTangent();
	btVector3 normal = getNormal();
	btVector3 binormal = getBinormal();

	wheelOffsets[0] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) + 0.5f/* + 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[1] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) - 0.5f/* - 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[2] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) + 0.5f/* + 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
	wheelOffsets[3] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) - 0.5f/* - 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
}

void Car::setUpWheelStuff(){

	/*
	kValue = abs((gravity.getY() * carMass ) / (restDisplacement * 4));
	critDampingValue = 2 * sqrt(kValue * carMass);
	hoverValue = btScalar(1.0f);
	*/
	kValue = 20.0f;
	critDampingValue = 2 * sqrt(kValue * 2.0);
	hoverValue = btScalar(1.0f);
}

void Car::observeVariables(ReloadEvent *e){
	for (int i = 0; i < 4; i++){
		newWheels[i].setCModifier(btScalar(e->numberHolder.cModifier));
		newWheels[i].setKModifier(btScalar(e->numberHolder.kModifier));
	}
}