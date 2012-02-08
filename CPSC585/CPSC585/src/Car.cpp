#include "Car.h"
#include "EventSystemHandler.h"


//                        ^ z
//                        |
//                        |
//            |------------------------|
//     wheel3 |           |            | wheel 2
//            |           |            |
//            |           |            |
//            |           |            |
//            |           |            |
//         x  |           |            |
//        <---|-----------.            |
//            |                        |
//            |                        |
//            |                        |
//            |                        |
//            |                        |
//     wheel1 |                        | wheel 0
//            |------------------------|
Car::Car(){	
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);	
	width = 5;
	height = 5;
	length = 10;
}

void Car::Observe(TriggerEvent *e){
	//position += tangent * e->getNormValue();//btVector3(e->getNormValue(), 0, 0);
}
	
/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Car::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	if(cShape != NULL)
	{
		btVector3 inertia;
		
		cShape->calculateLocalInertia(mass, inertia);
		
		
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,cShape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);

		wheels[0] = Spring(physicsObject);
		wheelOffsets[0] = btVector3(-width/2.f, -height/2.f, -(length/2.f) - 1.f);
		wheels[1] = Spring(physicsObject);
		wheelOffsets[1] = btVector3(width/2.f,-height/2.f, -(length/2.f) - 1.f);
		wheels[2] = Spring(physicsObject);
		wheelOffsets[2] = btVector3(-width/2.f,-height/2.f, (length/2.f) - 1.f);
		wheels[3] = Spring(physicsObject);		
		wheelOffsets[3] = btVector3(width/2.f,-height/2.f, (length/2.f) - 1.f);

		return true;
	}

	return false;
}

void Car::updateWheels()
{

	wheelOffsets[0] = (getNormal() * -height/2.0f) + (getBinormal() * +width/2.0f) + (getTangent() * (-length/2.0f - 1.0f));
	wheelOffsets[1] = (getNormal() * -height/2.0f) + (getBinormal() * -width/2.0f) + (getTangent() * (-length/2.0f - 1.0f));
	wheelOffsets[2] = (getNormal() * -height/2.0f) + (getBinormal() * +width/2.0f) + (getTangent() * (+length/2.0f - 1.0f));
	wheelOffsets[3] = (getNormal() * -height/2.0f) + (getBinormal() * -width/2.0f) + (getTangent() * (+length/2.0f - 1.0f));

	for (int i = 0; i < 4; i++){
		wheels[i].update(getPosition() + wheelOffsets[i], getNormal());
	}
}