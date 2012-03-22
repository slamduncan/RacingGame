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

bool SlowField::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	if(cShape != NULL)
	{
		//btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		// entRigidBodyCI(mass, motion state, collision shape, inertia);
		// need to update the mass to make it as a variable?
		//btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,cShape,btVector3(0, 0, 0));

		physicsObject = new btGhostObject();

		physicsObject->setCollisionShape(cShape);
		physicsObject->setWorldTransform(trans);

		blobContainer = (btCompoundShape*)cShape;

		//printf("I have internal type value: %i\n",physicsObject->getInternalType());
		//physicsObject->seta
		physicsObject->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);
		return true;
	}
	
	return false;
}
