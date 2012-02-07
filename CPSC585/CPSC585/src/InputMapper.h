#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H

#include "LinearMath/btQuaternion.h"
#include "AnalogEvent.h"
#include "TriggerEvent.h"
#include "MethodObserver.h"

class InputMapper{

	btQuaternion rotation;	

public:
	InputMapper();
	void updateRotation(double controllerInputX, double controllerInputY);
	void updateForwardForce(TriggerEvent *e);
	btQuaternion getRotaion();
	void updateRotation(AnalogEvent *e);
	MethodObserver<AnalogEvent, InputMapper> analogObserver;
	MethodObserver<TriggerEvent, InputMapper> triggerObserver;
};
#endif