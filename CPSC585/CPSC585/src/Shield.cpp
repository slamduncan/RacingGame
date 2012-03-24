#include "Shield.h"

Shield::Shield(Car *c)
{
	carRef = c;
	c->shieldActive = true;
	this->timeToSelfDestruct = clock() + 3 * CLOCKS_PER_SEC;
}

Shield::~Shield(){ carRef->shieldActive = false;}

void Shield::updateLocation()
{
	this->physicsObject->setWorldTransform(carRef->physicsObject->getWorldTransform());
}