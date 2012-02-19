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
	btRigidBody* chassis;
	
	btScalar kVal;
	
	//Variable added to test the new spring system.
	btScalar kValue;
	btScalar critDampingValue;
	btScalar hoverValue;
	btScalar carMass;
	btScalar restDisplacement;
	btVector3 gravity;

	Car();
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	Wheel newWheels[4];

	Spring wheels[4];
	btVector3 wheelOffsets[4];
	void updateWheels();
	void updateSpringLocations();

	void initObservers();

	void observeRotation(RotationEvent *e);
	void observeForwardForce(ForwardForceEvent *e);
	void observeVariables(ReloadEvent *e);

	void setUpWheelStuff();

	int getNextWaypointIndex();
	void setNextWaypointIndex(int in);

private:
	btScalar width, length, height;
	int nextWaypoint;

	// 3 power up slots, circular array of powerups?

	//Observers
	MethodObserver<RotationEvent, Car> rotationObserver;
	MethodObserver<ForwardForceEvent, Car> forwardForceObserver;
	MethodObserver<ReloadEvent, Car> updateVariableObserver;


};
#endif