#ifndef FORWARDFORCEEVENT_H
#define FORWARDFORCE_H
#include "Event.h"
#include "EventTypes.h"
#include "LinearMath/btVector3.h"

class ForwardForceEvent : public Event{

private:
	//btQuaternion rotation;	
	btScalar forceIn;
	btScalar forceNormIn;

public:	
	ForwardForceEvent(btScalar force, btScalar forceNorm);
	EventTypes::EventType ForwardForceEvent::getType() const{
		return EventTypes::TRIGGER;
	}
	ForwardForceEvent(){}
	btScalar getForce();
	btScalar getNormForce();
	

};
#endif