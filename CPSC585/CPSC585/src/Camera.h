#ifndef CAMERA_H
#define CAMERA_H

#include "MethodObserver.h"
#include "RightAnalogEvent.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btMatrix3x3.h"
#include "Car.h"

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
	
	void updateCamera(btTransform &transform);
	void updateCamera(btVector3 &lookAtPointIn, btVector3 &alignVector);

	void setTrackCar(Car* c);

	btVector3 offset;

	btVector3 up, lookAtPoint, cameraPosition, normal, lookAtVector;

	btScalar angle;

private:
	MethodObserver<RightAnalogEvent, Camera> analogObserver;
	void rotateCamera(RightAnalogEvent *e);
	Car* trackCar;
	btVector3 lastLookatPoint;
	btVector3 oldCameraPos;
	bool lookedBack;
	void computeCameraPosition();

	btVector3 UPVECTOR;

};
#endif 