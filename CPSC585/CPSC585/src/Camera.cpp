#include "Camera.h"

Camera::Camera(): analogObserver(this, &Camera::rotateCamera){

}

Camera::Camera(btVector3 &upIn, btVector3 &lookAtPointIn, btVector3 &cameraPositionIn) : analogObserver(this, & Camera::rotateCamera){
	up = upIn;
	lookAtPoint = lookAtPointIn;
	cameraPosition = cameraPositionIn;
}

void Camera::rotateCamera(RightAnalogEvent *e){

}