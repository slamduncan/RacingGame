#ifndef TYPEDOBSERVER_H
#define TYPEDOBSERVER_H
#include "Observer.h"


template <class ET> //ET = EventType, e.g. button Event
class TypedObserver : public Observer {
public:
	void Observe(Event *e){
		Observe(static_cast<ET*>(e));
		//observerType = e->getType();
	}

	virtual void Observe (ET *e) = 0;

	virtual EventTypes::EventType getSupportedType() const {
		return ET().getType();
	}
};
#endif