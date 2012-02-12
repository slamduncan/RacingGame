#ifndef INPUTMAPPER_H
#define INPUTMAPPER_H

#include "LinearMath/btQuaternion.h"
#include "LeftAnalogEvent.h"
#include "TriggerEvent.h"
#include "ReloadEvent.h"
#include "MethodObserver.h"

class InputMapper{
private:
	btQuaternion rotation;	
	TriggerEvent* lastTriggerEvent;
	float rotationModifier;

public:
	InputMapper();
	void updateRotation(double controllerInputX, double controllerInputY);
	void updateForwardForce(TriggerEvent *e);
	btQuaternion getRotaion();
	void updateRotation(LeftAnalogEvent *e);
	void updateVariables(ReloadEvent *e);
	MethodObserver<LeftAnalogEvent, InputMapper> analogObserver;
	MethodObserver<TriggerEvent, InputMapper> triggerObserver;
	MethodObserver<ReloadEvent, InputMapper> variableObserver;
};
#endif