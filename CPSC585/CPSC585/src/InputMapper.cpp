#include "InputMapper.h"
#include "EventSystemHandler.h"
#include "RotationEvent.h"
#include "ForwardForceEvent.h"

void InputMapper::updateRotation(double controllerInputX, double controllerInputY){
	rotation = btQuaternion((float)controllerInputX, (float)controllerInputY, 0, 0);

}

void InputMapper::updateRotation(AnalogEvent *e){
//	rotation = btQuaternion(e->getXVal(), e->getYVal(), 0, 0);	
	rotation = btQuaternion(0, -(float)(e->getXVal()), 0, 0);	
	//rotation.normalize();
	rotation /= 2000;
	EventSystemHandler::getInstance()->emitEvent(new RotationEvent(rotation));
}

void InputMapper::updateForwardForce(TriggerEvent *e){
	EventSystemHandler::getInstance()->emitEvent(new ForwardForceEvent(btScalar(e->getValue()), btScalar(e->getNormValue())));
}

btQuaternion InputMapper::getRotaion(){return rotation;}

InputMapper::InputMapper() : analogObserver(this, &InputMapper::updateRotation), triggerObserver(this, &InputMapper::updateForwardForce)
{	
	analogObserver.init(EventTypes::ANALOG);
	triggerObserver.init(EventTypes::TRIGGER);
}