#include "Event.h"
#include "EventTypes.h"

#ifndef OBSERVER_H
#define OBSERVER_H
class Observer
{
public:
	virtual void Observe(Event *e) = 0;
	virtual EventTypes::EventType getSupportedType() const = 0;	
};
#endif