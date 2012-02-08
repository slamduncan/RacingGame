#ifndef EVENTTYPES_H
#define EVENTTYPES_H

class EventTypes{
public:
	enum EventType {
		BUTTON,
		TRIGGER,
		ANALOG,
		RIGHT_ANALOG,
		LEFT_ANALOG,
		ROTATION,
		FORWARD_FORCE,
		EVENTS_NUM		
	};
};
#endif