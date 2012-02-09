#include "Camera.h"

Camera::Camera(): analogObserver(this, &Camera::rotateCamera){
	analogObserver.init(EventTypes::RIGHT_ANALOG);
}

Camera::Camera(btVector3 &upIn, btVector3 &lookAtPointIn, btVector3 &cameraPositionIn) : analogObserver(this, & Camera::rotateCamera){
	up = upIn;
	lookAtPoint = lookAtPointIn;
	cameraPosition = cameraPositionIn;

	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(up)).cross(lookAtVector)).normalize();
	analogObserver.init(EventTypes::RIGHT_ANALOG);

}

void Camera::rotateCamera(RightAnalogEvent *e){
	if (!e->isClicked())
		lookAtPoint = btVector3(lookAtPoint.getX(), lookAtPoint.getY() - e->getNormY(), lookAtPoint.getZ()-e->getNormX());
	else
		cameraPosition = btVector3(cameraPosition.getX(), cameraPosition.getY()- e->getNormY(), cameraPosition.getZ()-e->getNormX());
}

void Camera::setUpCamera(){	

	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(up)).cross(lookAtVector)).normalize();
	
	//btVector3 up(0, 1, 0);
	//btVector3 lookAtVector = (lookAtPoint - pos).normalize();

	//btVector3 binormal = lookAtVector.cross(up);

	//btVector3 normal = (binormal.cross(lookAtVector)).normalize();
}

void Camera::setUpCamera(btVector3 &lookAtPointIn){
	lookAtPoint = lookAtPointIn;
	setUpCamera();
}

void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPositionIn){
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;
	setUpCamera();
}

void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPositionIn, btVector3 &upIn){
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;
	up = upIn;
	setUpCamera();
}