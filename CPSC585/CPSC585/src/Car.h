#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "Entity.h"
#include "Spring.h"

class Car : public TypedObserver<TriggerEvent>, public Entity
{

public:
	//Car(btVector3 startingPos);
	Car();
	void Observe(TriggerEvent *e);

	//bool initRenderObject(char* filename);
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	Spring wheels[4];
	btVector3 wheelOffsets[4];
	void updateWheels();
	void updateSpringLocations();

private:
	btScalar width, length, height;
	// 3 power up slots, circular array of powerups?


};
#endif