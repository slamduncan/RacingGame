#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "Entity.h"

class Car : public TypedObserver<TriggerEvent>, public Entity{

public:
	//Car(btVector3 startingPos);
	Car();
	void Observe(TriggerEvent *e);

private:
	// 3 power up slots, circular array of powerups?


};
#endif