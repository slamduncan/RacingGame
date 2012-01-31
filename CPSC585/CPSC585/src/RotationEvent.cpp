#include "RotationEvent.h"

RotationEvent::RotationEvent(btQuaternion& rotIn){rotation = rotIn;}

btQuaternion RotationEvent::getQuaternion(){return rotation;}