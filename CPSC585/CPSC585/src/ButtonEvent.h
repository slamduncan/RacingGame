#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

#include "Event.h"
#include "EventTypes.h"

class ButtonEvent : public Event{

public:
	int buttonPressed;
	ButtonEvent(int buttonIn);
	EventTypes::EventType ButtonEvent::getType() const{
		return EventTypes::BUTTON;
	}
	ButtonEvent(){}
};
#endif