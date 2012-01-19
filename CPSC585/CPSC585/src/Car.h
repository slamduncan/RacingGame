#ifndef CAR_H
#define CAR_H

#include "TypedObserver.h"
#include "TriggerEvent.h"

class Car : public TypedObserver<TriggerEvent>{

public:
	struct Position{
		float y_pos;
		float x_pos;
		float z_pos;
	};
	Car(Position startingPos);
	Car();
	Position getPos();
	void Observe(TriggerEvent *e);

private:
	Position pos;

};
#endif