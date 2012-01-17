#include "ButtonEvent.h"
#include "EventTypes.h"

ButtonEvent::ButtonEvent(int buttonIn){
	buttonPressed = buttonIn;
}

EventTypes::EventType ButtonEvent::getType(){
	return EventTypes::BUTTON;
}