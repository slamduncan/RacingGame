#include "Car.h"
#include "EventSystemHandler.h"

//Car::Car(btVector3 startingPos){
//	position = startingPos;
//	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
//}

Car::Car(){	
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
}

btVector3 Car::getPos(){
	return pos;
}

void Car::Observe(TriggerEvent *e){
	position += btVector3(e->getNormValue()*100, 0, 0);
}