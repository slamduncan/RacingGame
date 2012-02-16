#include "Light.h"

Light::Light(void)
{
	position = btVector3(0, 0, 0);

}

Light::~Light(void)
{
}

Light::Light(btVector3 &initPos)
{
	position = initPos;
}

btVector3 Light::getPosition()
{
	return position;
}
void Light::setPosition(btVector3 &pos)
{
	position = pos;
}