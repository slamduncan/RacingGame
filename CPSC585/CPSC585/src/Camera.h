#ifndef CAMERA_H
#define CAMERA_H

#include "MethodObserver.h"
#include "RightAnalogEvent.h"
class Camera {
public:
	Camera();


private:
	MethodObserver<RightAnalogEvent, Camera> analogObserver;
	void rotateCamera(RightAnalogEvent *e);


};
#endif 