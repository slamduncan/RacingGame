#include "Camera.h"

Camera::Camera(): analogObserver(this, &Camera::rotateCamera){
	analogObserver.init(EventTypes::RIGHT_ANALOG);

	angle = 0;

	UPVECTOR = btVector3(0, 1, 0);
}

Camera::Camera(btVector3 &upIn, btVector3 &lookAtPointIn, btVector3 &cameraPositionIn) : analogObserver(this, & Camera::rotateCamera){
	up = upIn;
	lookAtPoint = lookAtPointIn;
	cameraPosition = cameraPositionIn;

	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(up)).cross(lookAtVector)).normalize();
	analogObserver.init(EventTypes::RIGHT_ANALOG);

	angle = 0;

	UPVECTOR = btVector3(0, 1, 0);
}

Camera::Camera(btVector3 &lookAtPointIn, btVector3& offset) : analogObserver(this, & Camera::rotateCamera)
{
	this->offset = offset;
	lookAtPoint = lookAtPointIn;
}

void Camera::rotateCamera(RightAnalogEvent *e){
	
	angle = (float)e->getNormX() * SIMD_RADS_PER_DEG * 3.0f;

	computeCameraPosition();

	//printf("X is%f\n", e->getNormX());
	/*
	if (!e->isClicked())
		lookAtPoint = btVector3(lookAtPoint.getX(), lookAtPoint.getY() - e->getNormY(), lookAtPoint.getZ()-e->getNormX());
	else
		cameraPosition = btVector3(cameraPosition.getX(), cameraPosition.getY()- e->getNormY(), cameraPosition.getZ()-e->getNormX());
	*/
}

Camera::Camera(btVector3 &cameraPositionIn, btVector3 &lookAtPointIn, btScalar distance, btScalar height) : analogObserver(this, & Camera::rotateCamera)
{
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;

}

void Camera::computeCameraPosition()
{
	//offset = cameraPosition - lookAtPoint;

	offset = offset.rotate(UPVECTOR, angle);

	cameraPosition = lookAtPoint + offset;
}

void Camera::setUpCamera(){	

	computeCameraPosition();
	
	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(UPVECTOR)).cross(lookAtVector)).normalize();

	//btVector3 up(0, 1, 0);
	//btVector3 lookAtVector = (lookAtPoint - pos).normalize();

	//btVector3 binormal = lookAtVector.cross(up);

	//btVector3 normal = (binormal.cross(lookAtVector)).normalize();
}

void Camera::setUpCamera(btVector3 &lookAtPointIn){
	lookAtPoint = lookAtPointIn;
	setUpCamera();
}

void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &offset){
	this->offset = offset;
	lookAtPoint = lookAtPointIn;
	setUpCamera();
}

/*
void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPositionIn){
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;
	setUpCamera();
}*/

void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPositionIn, btVector3 &upIn){
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;
	up = upIn;
	setUpCamera();
}

void Camera::debug()
{
	printf("pos: (%f, %f, %f)\n", cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
	printf("lookAT: (%f, %f, %f)\n", lookAtPoint.x(), lookAtPoint.y(), lookAtPoint.z());
}