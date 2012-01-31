#ifndef ROTATIONEVENT_H
#define ROTATIONEVENT_H
#include "Event.h"
#include "EventTypes.h"
#include "LinearMath/btQuaternion.h"

class RotationEvent : public Event{

private:
	btQuaternion rotation;	

public:	
	RotationEvent(btQuaternion& rotIn);
	EventTypes::EventType RotationEvent::getType() const{
		return EventTypes::ROTATION;
	}
	RotationEvent(){}
	btQuaternion getQuaternion();
	

};
#endif