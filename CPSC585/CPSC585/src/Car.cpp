#include "Car.h"
#include "EventSystemHandler.h"

Car::Car(Position startingPos){
	pos = startingPos;
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
}

Car::Car(){
	pos.y_pos = pos.x_pos = pos.z_pos = 0.0;
	EventSystemHandler::getInstance()->addObserver(this, EventTypes::TRIGGER);
}

Car::Position Car::getPos(){
	return pos;
}

void Car::Observe(TriggerEvent *e){
	pos.x_pos += e->getNormValue();
}