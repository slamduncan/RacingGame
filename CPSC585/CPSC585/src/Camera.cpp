#include "Camera.h"

Camera::Camera(): analogObserver(this, &Camera::rotateCamera){
	analogObserver.init(EventTypes::RIGHT_ANALOG);

	angle = 0;
}

Camera::Camera(btVector3 &upIn, btVector3 &lookAtPointIn, btVector3 &cameraPositionIn) : analogObserver(this, & Camera::rotateCamera){
	up = upIn;
	lookAtPoint = lookAtPointIn;
	cameraPosition = cameraPositionIn;

	lookAtVector = (lookAtPoint - cameraPosition).normalize();
	normal = ((lookAtVector.cross(up)).cross(lookAtVector)).normalize();
	analogObserver.init(EventTypes::RIGHT_ANALOG);

	angle = 0;

}

void Camera::rotateCamera(RightAnalogEvent *e){
	
	angle = (float)e->getNormX() * SIMD_RADS_PER_DEG * 3.0f;

	printf("angle = %f", angle);

	btScalar xRot = cosf(angle); 
	btScalar zRot = sinf(angle);

	btVector3 viewVec = btVector3(xRot, 0, zRot);

	//btScalar yOffset = ((lookAtPoint - cameraPosition).length()).getY();

	btVector3 temp = (lookAtPoint - cameraPosition);

	temp.rotate(btVector3(0, 1, 0), angle);
/*
	(viewVec.setY(-yOffset));

	cameraPosition = viewVec + lookAtPoint;
*/
	cameraPosition = lookAtPoint - temp;


	//printf("X is%f\n", e->getNormX());
	/*
	if (!e->isClicked())
		lookAtPoint = btVector3(lookAtPoint.getX(), lookAtPoint.getY() - e->getNormY(), lookAtPoint.getZ()-e->getNormX());
	else
		cameraPosition = btVector3(cameraPosition.getX(), cameraPosition.getY()- e->getNormY(), cameraPosition.getZ()-e->getNormX());
	*/
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