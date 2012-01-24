#ifndef TRIGGEREVENT_H
#define TRIGGEREVENT_H
#include "Event.h"
#include "EventTypes.h"

class TriggerEvent : public Event{

public:	
	TriggerEvent(int triggerValIn, double normTriggerValIn);
	EventTypes::EventType TriggerEvent::getType() const{
		return EventTypes::TRIGGER;
	}
	TriggerEvent(){};
	int getValue(){return triggerValue;}
	double getNormValue(){return normTriggerVale;}
	void setValue(int in){triggerValue = in;}
	void setNormValue(double in){normTriggerVale = in;}

private:
	int triggerValue;
	double normTriggerVale;

};
#endif