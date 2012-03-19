#include "SlowField.h"

SlowField::SlowField(Car* c)
{
	car = c;
	btGhostObject * slowBlobContainer;
	slowBlobContainer = new btGhostObject();
	btCompoundShape* blobContainer;
	blobContainer = new btCompoundShape();
	slowBlobContainer->setCollisionShape(blobContainer);
	//slowBlobContainer->setWorldTransform(physicsObject->getWorldTransform());
	//slowBlobContainer->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);

	Physics * phys = Physics::Inst();			

	phys->addGhost(slowBlobContainer);
}

SlowField::~SlowField(void)
{
}
