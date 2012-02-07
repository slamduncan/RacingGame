#include "TriggerEvent.h"

TriggerEvent::TriggerEvent(int triggerValIn, double normTriggerValIn){
	triggerValue = triggerValIn;
	normTriggerVale = normTriggerValIn;
}

TriggerEvent::TriggerEvent(const TriggerEvent& inTrigger){
	triggerValue = inTrigger.triggerValue;
	normTriggerVale = inTrigger.normTriggerVale;
}
