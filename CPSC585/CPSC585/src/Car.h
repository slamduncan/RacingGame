#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "RotationEvent.h"
#include "ForwardForceEvent.h"
#include "Entity.h"
#include "Spring.h"

class Car : public Entity
{

public:
	//Car(btVector3 startingPos);
	Car();
	//void Observe(TriggerEvent *e);

	//bool initRenderObject(char* filename);
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	Spring wheels[4];
	btVector3 wheelOffsets[4];
	void updateWheels();
	void updateSpringLocations();

	void initObservers();

	void observeRotation(RotationEvent *e);
	void observeForwardForce(ForwardForceEvent *e);


private:
	btScalar width, length, height;
	// 3 power up slots, circular array of powerups?

	//Observers
	MethodObserver<RotationEvent, Car> rotationObserver;
	MethodObserver<ForwardForceEvent, Car> forwardForceObserver;


};
#endif