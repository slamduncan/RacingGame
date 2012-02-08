#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H

#include "LinearMath/btQuaternion.h"
#include "LeftAnalogEvent.h"
#include "TriggerEvent.h"
#include "MethodObserver.h"

class InputMapper{

	btQuaternion rotation;	
	TriggerEvent* lastTriggerEvent;

public:
	InputMapper();
	void updateRotation(double controllerInputX, double controllerInputY);
	void updateForwardForce(TriggerEvent *e);
	btQuaternion getRotaion();
	void updateRotation(LeftAnalogEvent *e);
	MethodObserver<LeftAnalogEvent, InputMapper> analogObserver;
	MethodObserver<TriggerEvent, InputMapper> triggerObserver;
};
#endif