#ifndef RELOADEVENT_H
#define RELOADEVENT_H

#include "Event.h"
#include "ProjectNumbers.h"
#include "EventTypes.h"
class ReloadEvent : public Event {
public:
	ProjectNumbers numberHolder;

	ReloadEvent() : numberHolder(){
		//numberHolder.readVariablesIn();
	}

	void read()
	{
		numberHolder.readVariablesIn();
	}

	EventTypes::EventType getType() const {return EventTypes::RELOAD_VARIABLES;} 

};
#endif