#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "RotationEvent.h"
#include "ForwardForceEvent.h"
#include "Entity.h"
#include "Spring.h"
#include "Wheel.h"

class Car : public Entity
{

public:
	btScalar kVal;
	
	//Variable added to test the new spring system.
	btScalar kValue;
	btScalar critDampingValue;
	btScalar hoverValue;
	btScalar carMass;
	btScalar restDisplacement;
	btVector3 gravity;
	//Car(btVector3 startingPos);
	Car();
	//void Observe(TriggerEvent *e);

	//bool initRenderObject(char* filename);
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	Wheel newWheels[4];

	Spring wheels[4];
	btVector3 wheelOffsets[4];
	void updateWheels();
	void updateSpringLocations();

	void initObservers();

	void observeRotation(RotationEvent *e);
	void observeForwardForce(ForwardForceEvent *e);

	void setUpWheelStuff();
	void cheatAndFixRotation();

private:
	btScalar width, length, height;

	// 3 power up slots, circular array of powerups?

	//Observers
	MethodObserver<RotationEvent, Car> rotationObserver;
	MethodObserver<ForwardForceEvent, Car> forwardForceObserver;


};
#endif