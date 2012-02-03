#ifndef FORWARDFORCEEVENT_H
#define FORWARDFORCEEVENT_H
#include "Event.h"
#include "EventTypes.h"

class ForwardForceEvent : public Event{

private:
	btScalar force, normForce;	

public:	
	ForwardForceEvent(btScalar in, btScalar normIn){force = in; normForce = normIn;}
	EventTypes::EventType ForwardForceEvent::getType() const{
		return EventTypes::FORWARD_FORCE;
	}
	ForwardForceEvent(){};
	btScalar getForce(){return force;}	
	btScalar getNormForce(){return normForce;}


};
#endif