#ifndef RIGHTANALOGEVENT_H
#define RIGHTANALOGEVENT_H
#include "AnalogEvent.h"

class RightAnalogEvent : public AnalogEvent {
public:
	RightAnalogEvent(){};
	RightAnalogEvent(int xval,double normXVal, int yval,double normYVal, bool clickedIn):AnalogEvent(xval, normXVal, yval, normYVal, clickedIn){};
	EventTypes::EventType RightAnalogEvent::getType() const{
		return EventTypes::RIGHT_ANALOG;
	}
};
#endif