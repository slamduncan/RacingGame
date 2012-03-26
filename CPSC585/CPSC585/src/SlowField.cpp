#include "SlowField.h"

SlowField::SlowField(Car* c)
{
	car = c;
	//slowBlobContainer = new btGhostObject();
	//blobContainer = new btCompoundShape();
	//slowBlobContainer->setCollisionShape(blobContainer);
	//slowBlobContainer->setWorldTransform(physicsObject->getWorldTransform());
	//slowBlobContainer->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);

	//Physics * phys = Physics::Inst();			

	//phys->addGhost(slowBlobContainer);

	//timeToSelfDestruct = clock() + 9*CLOCKS_PER_SEC;
	timeToDrop = clock() + 0.10*CLOCKS_PER_SEC;
	timeToDelete = clock() + 4.5*CLOCKS_PER_SEC;
	numSpawned = 0;
	numDeleted = 0;
}

SlowField::~SlowField(void)
{
}

Car* SlowField::getCar(){
	return car;
}

btScalar* SlowField::getChildGLMatrix(int index)
{
	btTransform childT = blobContainer->getChildTransform(index);	
	btTransform containerT = physicsObject->getWorldTransform();

	btVector3 containerPos = containerT.getOrigin();
	btVector3 childPos = childT.getOrigin();

	btVector3 pos = childPos+containerPos;

	childT = containerT*childT;

	childT.getOpenGLMatrix(glMatrix);

	return glMatrix;
}

bool SlowField::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	if(cShape != NULL)
	{
		physicsObject = new btGhostObject();

		physicsObject->setCollisionShape(cShape);
		physicsObject->setWorldTransform(trans);

		blobContainer = (btCompoundShape*)cShape;

		physicsObject->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);
		return true;
	}
	
	return false;
}
