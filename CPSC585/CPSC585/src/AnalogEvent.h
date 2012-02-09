#ifndef ANALOGEVENT_H
#define ANALOGEVENT_H
#include "Event.h"
#include "EventTypes.h"

class AnalogEvent : public Event{

public:	
	AnalogEvent(int xval,double normXVal, int yval,double normYVal, bool clickedIn);
	EventTypes::EventType AnalogEvent::getType() const{
		return EventTypes::ANALOG;
	}
	AnalogEvent(){}
	int getXVal(){return XValue;}
	int getYVal(){return YValue;}
	double getNormY(){return normYValue;}
	double getNormX(){return normXValue;}
	bool isClicked(){return clicked;}

private:
	int XValue, YValue;
	double normXValue, normYValue;
	bool clicked;

};
#endif