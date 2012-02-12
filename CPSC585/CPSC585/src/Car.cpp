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
Car::Car() : rotationObserver(this, &Car::observeRotation), forwardForceObserver(this, &Car::observeForwardForce)
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
//		physicsObject->setAngularFactor(1000);
	
	btVector3 test = e->getQuaternion().getAxis();
	btVector3 temp = physicsObject->getAngularVelocity();
	if (temp.length() < 5)
	//physicsObject->setAngularVelocity(test);
		physicsObject->applyTorque(-test);		
}

void Car::observeForwardForce(ForwardForceEvent *e){
	btVector3 tan = getTangent() * (e->getNormForce());
	//tan /= 10.0f;

	
	//btVector3 UP = btVector3(0, 1, 0);
	//btScalar projVec = tan.dot(UP);
	//btVector3 offset = UP * -projVec;

	tan.setY(0);

	physicsObject->applyImpulse(tan,getPosition()-btVector3(0,1,0));
	//physicsObject->applyCentralImpulse(tan);
	

	
	//physicsObject->applyCentralImpulse(e->getNormForce()*btVector3(1,0,0));
	
	//btVector3 temp = physicsObject->getAngularFactor();
	
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
	carMass = mass;
	kVal = (mass * 10.0f)/(3.0f * 4.0f);
	if(cShape != NULL)
	{
		btVector3 inertia;
		
		cShape->calculateLocalInertia(mass, inertia);
		
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);
		//entMotionState->setWorldTransform(trans);
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,cShape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);
		physicsObject->setLinearVelocity(btVector3(0,0,0));
		
		wheels[0] = Spring(physicsObject, kVal);
		//wheelOffsets[0] = btVector3(-width/2.f, -height/2.f, -(length/2.f) + 1.f);
		wheels[1] = Spring(physicsObject, kVal);
		//wheelOffsets[1] = btVector3(width/2.f,-height/2.f, -(length/2.f) + 1.f);
		wheels[2] = Spring(physicsObject, kVal);
		//wheelOffsets[2] = btVector3(-width/2.f,-height/2.f, (length/2.f) - 1.f);
		wheels[3] = Spring(physicsObject, kVal);	
		//wheelOffsets[3] = btVector3(width/2.f,-height/2.f, (length/2.f) - 1.f);

		updateSpringLocations();
		setUpWheelStuff();
		btScalar wheelLength(5.0f);
		
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
		//wheelOffsets[0] = btVector3(-width/2.f, -height/2.f, -(length/2.f) + 1.f);
		newWheels[1] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue, gravity, getPosition() + wheelOffsets[1],
			getPosition() + wheelOffsets[1] - getNormal()*3.0f, physicsObject);
		//wheelOffsets[1] = btVector3(width/2.f,-height/2.f, -(length/2.f) + 1.f);
		newWheels[2] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue,critDampingValue, gravity, getPosition() + wheelOffsets[2],
			getPosition() + wheelOffsets[2] - getNormal()*3.0f, physicsObject);
		//wheelOffsets[2] = btVector3(-width/2.f,-height/2.f, (length/2.f) - 1.f);
		newWheels[3] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue,gravity, getPosition() + wheelOffsets[3],
			getPosition() + wheelOffsets[3] - getNormal()*3.0f, physicsObject);			

		
//		physicsObject->setDamping(0, 0.75);

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

	btVector3 forces [4];
	for (int i = 0; i < 4; i++){
		forces[i] = newWheels[i].calcForce(getPosition() + wheelOffsets[i], getNormal());
	}

	for (int i = 0; i < 4; i++){
		btVector3 contact = newWheels[i].getBottomSpringPosition();
		physicsObject->applyImpulse(forces[i],contact-getPosition());
	}

	//printf("\n--------------------------------------------------\n");

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
	kValue = 100.0/8.0;
	critDampingValue = 2 * sqrt(kValue * 2.0);
	hoverValue = btScalar(1.0f);
}

void Car::cheatAndFixRotation(){
	btVector3 A = this->getNormal();
	btVector3 B = btVector3(0,1,0);

	float angle = acos(A.dot(B))*180.0/M_PI;
	printf("%f\n",angle);

	if(angle > 30){
		physicsObject->setAngularVelocity(btVector3(0,0,0));
	}
}