#ifndef LEFTANALOGEVENT_H
#define LEFTANALOGEVENT_H
#include "AnalogEvent.h"

class LeftAnalogEvent : public AnalogEvent {
public:
	LeftAnalogEvent(){};
	LeftAnalogEvent(int xval,float normXVal, int yval,float normYVal, bool clickedIn):AnalogEvent(xval, normXVal, yval, normYVal, clickedIn){};
	EventTypes::EventType LeftAnalogEvent::getType() const{
		return EventTypes::LEFT_ANALOG;
	}
};
#endif