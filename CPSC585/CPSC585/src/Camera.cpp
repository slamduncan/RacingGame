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

	//printf("(%f, 0, %f)\n", e->getNormX(), e->getNormY());
	
	btVector3 target = btVector3(e->getNormX(), 0, e->getNormY());

	if(!target.isZero())
	{
		
		btVector3 projOffset = offset;
		projOffset.setY(btScalar(0.f));

		btScalar length = projOffset.length();

		target = target.normalized() * length;

		//target = target.rotate(UPVECTOR, 90);

		cameraPosition = target + lookAtPoint;

		lookAtVector = lookAtPoint - cameraPosition;

		normal = lookAtVector.cross(UPVECTOR);

		normal = normal.cross(lookAtVector).normalized();
		
	}
	
/*
	cameraPosition = cameraPosition.lerp(target, 0.05f);

	lookAtVector = lookAtPoint - cameraPosition;

	normal = lookAtVector.cross(UPVECTOR);

	normal = normal.cross(lookAtVector).normalized();
*/	
/*	
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
*/	
}

Camera::Camera(btVector3 &cameraPositionIn, btVector3 &lookAtPointIn, btScalar distance, btScalar height) : analogObserver(this, & Camera::rotateCamera)
{
	cameraPosition = cameraPositionIn;
	lookAtPoint = lookAtPointIn;

}

void Camera::updateCamera(btTransform &transform)
{
	btVector3 target = transform * offset;

	lookAtPoint = transform.getOrigin();

	int interpelationCount = 0;
	cameraPosition = cameraPosition.lerp(target, 0.01f);
	while((cameraPosition - lookAtPoint).length() > 50 && interpelationCount < 20)
	{
		cameraPosition = cameraPosition.lerp(target, 0.01f);
		interpelationCount++;
	}

	lookAtVector = lookAtPoint - cameraPosition;

	normal = lookAtVector.cross(UPVECTOR);

	normal = normal.cross(lookAtVector).normalized();

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

	lookAtPoint = lookAtPointIn;	// car position
	
	btVector3 offNormaled = offset.normalized();

	btScalar offsetAmount = offNormaled.dot(-alignVector);

	btScalar offsetY = offset.getY();

	if(offsetAmount < btScalar(0.f))
	{
		//btVector offsetVector = UPVECTOR.cross(alignVector);

		btVector3 alignment = offset + alignVector;

		//btScalar angle = offset.angle(alignment);

		//alignment.setY(offsetY);

		alignment = alignment.normalized()*offset.length();

		offset = alignment;

		//offset.setY(10);

		offset = offset.normalized() * offset.length();

		cameraPosition = offset + lookAtPoint;

	}



	/*
	lookAtPoint = lookAtPointIn;
	btVector3 forwordVector = alignVector.cross(UPVECTOR);
	btVector3 oldCameraPostion = cameraPosition;

	
	btScalar offsetAmount = offset.length();

	offset.setY(0.0f);

	btVector3 PostionToBeAt = lookAtPointIn + (-forwordVector * offset.length()) + UPVECTOR * 10;
	cameraPosition = PostionToBeAt;
	*/
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
