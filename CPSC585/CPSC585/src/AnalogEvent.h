#ifndef ANALOGEVENT_H
#define ANALOGEVENT_H
#include "Event.h"
#include "EventTypes.h"

class AnalogEvent : public Event{

public:	
	AnalogEvent(int xval,float normXVal, int yval,float normYVal, bool clickedIn);
	EventTypes::EventType AnalogEvent::getType() const{
		return EventTypes::ANALOG;
	}
	AnalogEvent(){}
	int getXVal(){return XValue;}
	int getYVal(){return YValue;}
	float getNormY(){return normYValue;}
	float getNormX(){return normXValue;}
	bool isClicked(){return clicked;}

private:
	int XValue, YValue;
	float normXValue, normYValue;
	bool clicked;

};
#endif