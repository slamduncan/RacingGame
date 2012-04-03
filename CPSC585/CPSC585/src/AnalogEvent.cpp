#include "AnalogEvent.h"

AnalogEvent::AnalogEvent(int xval, float normXVal, int yval, float normYVal, bool clickedIn) 
		: XValue(xval), normXValue(normXVal), YValue(yval), normYValue(normYVal), clicked(clickedIn){}
