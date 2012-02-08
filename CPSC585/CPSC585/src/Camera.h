#ifndef CAMERA_H
#define CAMERA_H

#include "MethodObserver.h"
#include "RightAnalogEvent.h"
#include "LinearMath/btVector3.h"
class Camera {
public:
	Camera();
	//UPvector, Look at vector/point, cam pos
	Camera(btVector3 &upIn, btVector3 &lookAtPointIn, btVector3 &cameraPositionIn);


private:
	MethodObserver<RightAnalogEvent, Camera> analogObserver;
	void rotateCamera(RightAnalogEvent *e);
	btVector3 up, lookAtPoint, cameraPosition;


};
#endif 