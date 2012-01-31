#include "InputMapper.h"
#include "EventSystemHandler.h"
#include "RotationEvent.h"

void InputMapper::updateRotation(double controllerInputX, double controllerInputY){
	rotation = btQuaternion(controllerInputX, controllerInputY, 0, 0);

}

void InputMapper::updateRotation(AnalogEvent *e){
	rotation = btQuaternion(e->getXVal(), e->getYVal(), 0, 0);	
	//rotation.normalize();
	rotation /= 1000;
	EventSystemHandler::getInstance()->emitEvent(new RotationEvent(rotation));
}

btQuaternion InputMapper::getRotaion(){return rotation;}

InputMapper::InputMapper() : analogObserver(this, &InputMapper::updateRotation){}