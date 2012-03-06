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
	
	// cap the angle
	if(angle > SIMD_2_PI)
	{
	angle = btScalar(0.);
	}
	if(angle < btScalar(0.))
	{
	angle = SIMD_2_PI;
	}

	angle = (float)e->getNormX() * SIMD_RADS_PER_DEG * 3.0f;

	computeCameraPosition();
	
}

Camera::Camera(btVector3 &cameraPositionIn, btVector3 &lookAtPointIn, btScalar distance, btScalar height) : analogObserver(this, & Camera::rotateCamera)
{
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;

}

void Camera::updateCamera(btVector3 &lookAtPointIn, btVector3 &alignVector)
{
/*
	lookAtPoint = lookAtPointIn;

	btScalar offsetLength = offset.length();

	btScalar offsetAmount = alignVector.dot((cameraPosition - lookAtPointIn).normalize());

	btVector3 offsetVector = (offsetAmount/btScalar(10.0f)) * alignVector.normalize();

	//computeCameraPosition();

	cameraPosition = lookAtPoint + offset;

	cameraPosition += offsetVector;
*/

	lookAtPoint = lookAtPointIn;
	btVector3 forwordVector = alignVector.cross(UPVECTOR);
	btVector3 oldCameraPostion = cameraPosition;
	btVector3 PostionToBeAt = lookAtPointIn + (forwordVector * offset.x()) + UPVECTOR * offset.y();
	cameraPosition = PostionToBeAt;
/*
	btVector3 currentVector = cameraPosition - lookAtPointIn;
	btVector3 moveVector = currentVector.dot(alignVector) * alignVector;
	if (moveVector.length() < 0.001)
	{
		moveVector = btVector3(0,0,0);		
	}
	btVector3 newPosition = (cameraPosition - moveVector);
	btVector3 newPositionVector = newPosition - lookAtPointIn;
	btVector3 temp = newPositionVector.normalized()*offset.length();
	btVector3 vectorToNormalize = newPositionVector - newPositionVector.normalized()*offset.length();
	btVector3 finalPosition = newPosition - (vectorToNormalize);
	cameraPosition = finalPosition;

	if (newPositionVector.dot(forwordVector) < 0.001)
	{
		cameraPosition = oldCameraPostion;
	}
*/	
}

void Camera::computeCameraPosition()
{

	offset = offset.rotate(UPVECTOR, angle);

	cameraPosition = lookAtPoint + offset;
}

void Camera::setUpCamera(){	

	computeCameraPosition();
	
	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(UPVECTOR)).cross(lookAtVector)).normalize();
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

void Camera::setUpCamera(btVector3 &lookAtPointIn, btVector3 &cameraPositionIn, btVector3 &upIn){
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;
	up = upIn;
	setUpCamera();
}
