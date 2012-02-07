#ifndef LEFTANALOGEVENT_H
#define LEFTANALOGEVENT_H
#include "AnalogEvent.h"

class LeftAnalogEvent : public AnalogEvent {
public:
	LeftAnalogEvent(){};
	LeftAnalogEvent(int xval,double normXVal, int yval,double normYVal):AnalogEvent(xval, normXVal, yval, normYVal){};
	EventTypes::EventType LeftAnalogEvent::getType() const{
		return EventTypes::LEFT_ANALOG;
	}
};
#endif