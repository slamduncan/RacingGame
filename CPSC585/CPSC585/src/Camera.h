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
	
	Camera(btVector3 &cameraPositionIn, btVector3 &lookAtPointIn, btScalar distance, btScalar height);

	Camera(btVector3 &lookAtPointIn, btVector3& offset);

	void setUpCamera();
	void setUpCamera(btVector3 &lookAtPointIn);
	void setUpCamera(btVector3 &lookAtPointIn, btVector3 &offset);

	void setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPosition, btVector3 &upIn);


	btVector3 offset;

	btVector3 up, lookAtPoint, cameraPosition, normal, lookAtVector;

	btScalar angle;

private:
	MethodObserver<RightAnalogEvent, Camera> analogObserver;
	void rotateCamera(RightAnalogEvent *e);

	void computeCameraPosition();

	btVector3 UPVECTOR;

};
#endif 