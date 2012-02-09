#include "AnalogEvent.h"

AnalogEvent::AnalogEvent(int xval, double normXVal, int yval, double normYVal, bool clickedIn) 
		: XValue(xval), normXValue(normXVal), YValue(yval), normYValue(normYVal), clicked(clickedIn){}
