#include "Camera.h"

Camera::Camera(): analogObserver(this, &Camera::rotateCamera){

}

void Camera::rotateCamera(RightAnalogEvent *e){

}