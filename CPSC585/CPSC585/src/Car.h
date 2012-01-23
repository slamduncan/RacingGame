#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "Entity.h"
#include "LinearMath/btVector3.h"

class Car : public TypedObserver<TriggerEvent>, public Entity{

public:
	//Car(btVector3 startingPos);
	Car();
	btVector3 getPos();
	void Observe(TriggerEvent *e);

private:
	btVector3 pos;

};
#endif