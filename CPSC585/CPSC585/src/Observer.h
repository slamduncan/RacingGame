#include "Event.h"
#include "EventTypes.h"

#ifndef OBSERVER_H
#define OBSERVER_H
class Observer
{
public:
	virtual void Observe(Event *e) = 0;
	EventTypes::EventType observerType;
};
#endif