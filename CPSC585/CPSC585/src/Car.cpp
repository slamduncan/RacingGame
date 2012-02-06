#include "Car.h"
#include "EventSystemHandler.h"

Car::Car(){	
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
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

		return true;
	}

	return false;
}