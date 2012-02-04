#include "Car.h"
#include "EventSystemHandler.h"

Car::Car(){	
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
}

void Car::Observe(TriggerEvent *e){
	//position += tangent * e->getNormValue();//btVector3(e->getNormValue(), 0, 0);
}