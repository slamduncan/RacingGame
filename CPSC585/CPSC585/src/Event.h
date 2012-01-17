#ifndef EVENT_H
#define EVENT_H

#include "EventTypes.h"
class Event {
public:	
	virtual EventTypes::EventType getType() = 0;
};
#endif