#ifndef FORWARDFORCEEVENT_H
#include "Event.h"
#include "EventTypes.h"
#include "LinearMath/btVector3.h"

class ForwardForceEvent : public Event{

private:
	btScalar force;
	btScalar forceNormIn;

public:	
	ForwardForceEvent(btScalar force, btScalar forceNorm);
	EventTypes::EventType ForwardForceEvent::getType() const{
		return EventTypes::FORWARD_FORCE;
	}
	ForwardForceEvent(){}
	btScalar getForce();
	btScalar getNormForce();

};
#endif