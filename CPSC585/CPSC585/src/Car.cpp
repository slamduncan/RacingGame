#include "Car.h"
#include "EventSystemHandler.h"

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
Car::Car() : rotationObserver(this, &Car::observeRotation), forwardForceObserver(this, &Car::observeForwardForce)
{		
	width = 5;
	height = 5;
	length = 10;
}

void Car::initObservers()
{	
	rotationObserver.init(EventTypes::ROTATION);
	forwardForceObserver.init(EventTypes::FORWARD_FORCE);

}

void Car::observeRotation(RotationEvent *e){		
//		physicsObject->setAngularFactor(1000);
	
	btVector3 test = e->getQuaternion().getAxis();
	btVector3 temp = physicsObject->getAngularVelocity();
	if (temp.length() < 5)
	//physicsObject->setAngularVelocity(test);
		physicsObject->applyTorque(test);		
}

void Car::observeForwardForce(ForwardForceEvent *e){
	btVector3 tan = getTangent() * (e->getNormForce());
	tan /= 10.0f;

	btVector3 UP = btVector3(0, 1, 0);
	btScalar projVec = tan.dot(UP);
	btVector3 offset = UP * -projVec;

	physicsObject->applyCentralImpulse(tan + offset);
	btVector3 temp = physicsObject->getAngularFactor();
	
	//physicsObject->applyImpulse(tan + offset, getPosition() + getNormal() * -2.5f - getTangent() * 5.0f);
	//for (int i = 0; i < 4; i++)
	//	physicsObject->applyImpulse((tan + offset), wheelOffsets[i]);//getPosition()); 
	
}




/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Car::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	kVal = (mass * 30.0)/(3.0 * 4.0);
	if(cShape != NULL)
	{
		btVector3 inertia;
		
		cShape->calculateLocalInertia(mass, inertia);
		
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);
		//entMotionState->setWorldTransform(trans);
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,cShape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);
		
		wheels[0] = Spring(physicsObject, kVal);
		//wheelOffsets[0] = btVector3(-width/2.f, -height/2.f, -(length/2.f) + 1.f);
		wheels[1] = Spring(physicsObject, kVal);
		//wheelOffsets[1] = btVector3(width/2.f,-height/2.f, -(length/2.f) + 1.f);
		wheels[2] = Spring(physicsObject, kVal);
		//wheelOffsets[2] = btVector3(-width/2.f,-height/2.f, (length/2.f) - 1.f);
		wheels[3] = Spring(physicsObject, kVal);	
		//wheelOffsets[3] = btVector3(width/2.f,-height/2.f, (length/2.f) - 1.f);

		updateSpringLocations();
	
		/*
		for(int i = 0; i < 4; i++)
		{
			btVector3 temp = wheelOffsets[i];

			printf("(%f, %f, %f)\n", temp.x(), temp.y(), temp.z());
		}
		*/
		//physicsObject->setAngularFactor(btScalar(0.01f));


		return true;
	}

	return false;
}

void Car::updateWheels()
{
	updateSpringLocations();


	for (int i = 0; i < 4; i++){
		wheels[i].update(getPosition() + wheelOffsets[i], getNormal());
	}
}

void Car::updateSpringLocations()
{
	btVector3 position = getPosition();
	btVector3 tangent = getTangent();
	btVector3 normal = getNormal();
	btVector3 binormal = getBinormal();

	wheelOffsets[0] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) - 0.25f/* + 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[1] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) + 0.25f/* - 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[2] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) - 0.25f/* + 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
	wheelOffsets[3] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) + 0.25f/* - 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
}