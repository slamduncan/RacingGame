#include "Physics.h"
#include "EntityManager.h"
#include "Renderer.h"
#include "PowerUp.h"

Physics* Physics::physInstance = 0;
EntityManager* entityManager;
Renderer* physRender;

Physics* Physics::Inst(void){	
	if(physInstance == 0){
		physInstance = new Physics();
	}
	return physInstance;
}

Physics::Physics(void) : variableObserver(this, &Physics::updateVariables)
{
	entityManager = EntityManager::getInstance();
	physRender = Renderer::getInstance();

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

	variableObserver.init(EventTypes::RELOAD_VARIABLES);
}

Physics::~Physics(void)
{

	// Need to clean up the dynamic world since there are still entities in it
	dynamicsWorld->getCollisionObjectArray().clear();
	
	delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

//*********************************************************************************
//This code will either be completely replaced or wind up in car.cpp!
//Used for checking contact pairs
struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback {
	
	//! Constructor, pass whatever context you want to have available when processing contacts
	ContactSensorCallback(btCollisionObject& tgtBody, PowerUp* pUp/*, ... */)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), powerUp(pUp){}
	
	btCollisionObject& body; //!< The body the sensor is monitoring (in our case it's the car)
	PowerUp* powerUp;	//The powerup under consideration (wanted to avoid doing this..)

	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObject* colObj0,int partId0,int index0,
		const btCollisionObject* colObj1,int partId1,int index1)
	{
		btVector3 pt; // will be set to point of collision relative to body
		if(colObj0==&body) {
			pt = cp.m_localPointA;
		} else {
			assert(colObj1==&body && "body does not match either collision object");
			pt = cp.m_localPointB;
		}
		// do stuff
		//printf("I hit a thing.\n");
		powerUp->hitPowerup();

		return 0; // Return value not used for anything.
	}
};
//*********************************************************************************

void Physics::step()
{	
	dynamicsWorld->stepSimulation(1/60.f,10);

	for (int i = 0; i < entityManager->getCarList()->size(); i++){
		//update springs
		entityManager->getCarList()->at(i)->updateWheels();
		

		//***********************************************************
		//This code will be moved!! (Probably to a function powerUpCollide in Car...)
		//This code may be replaced for a more optimal version.

		//Check collision with any powerups
		btCollisionObject* carPointer = entityManager->getCar(i)->getPhysicsObject();
		
		//For each powerup...
		for(int j = 0; j < entityManager->getPowerUpList()->size(); j++){
			//Grab a pointer to it, perform the contact test
			btCollisionObject* pUpPointer = entityManager->getPowerup(j)->getPhysicsObject();
			ContactSensorCallback callback(*carPointer, entityManager->getPowerup(j));

			dynamicsWorld->contactPairTest(carPointer,pUpPointer,callback);

			//Contact test will set the powerup's "collected" variable! (collected or not collected)
			//If we hit this powerup:
			if(entityManager->getPowerup(j)->isHit()){
				//added = 1 if was collected, 0 otherwise.
				int added = entityManager->getCar(i)->AddPowerUp(1);
				//printf("Now I have %i powerups!\n",entityManager->getCar(i)->GetNumberPowerUps());

				//Do we want the powerup to vanish anyway if a car has full powerups??

				//TODO: DECONSTRUCT THE POWERUP! (Or start a respawn timer?)
				entityManager->getPowerUpList()->remove(entityManager->getPowerup(j));
				break;
			}else{
				//printf("I didn't hit anything, but I have %i powerups.\n",entityManager->getCar(i)->GetNumberPowerUps());
			}
		}
		//***********************************************************
	}

}

void Physics::setGravity(const btVector3 &gravityIn)
{
	gravity = gravityIn;
	dynamicsWorld->setGravity(gravityIn);
}

void Physics::addEntity(const Entity &ent)
{
	//dynamicsWorld->addRigidBody(btRigidBody::upcast(ent.physicsObject));
	dynamicsWorld->addCollisionObject(ent.physicsObject);
}

void Physics::removeEntity(const Entity &ent)
{
	//dynamicsWorld->removeRigidBody(btRigidBody::upcast(ent.physicsObject));
	dynamicsWorld->removeCollisionObject(ent.physicsObject);
}

void Physics::addRigidBody(const Entity &entity)
{
	dynamicsWorld->addRigidBody(btRigidBody::upcast(entity.physicsObject));
}

void Physics::removeRigidBody(const Entity &entity)
{
	dynamicsWorld->removeRigidBody(btRigidBody::upcast(entity.physicsObject));
}

void Physics::setDebugDrawer(btIDebugDraw *debugDrawer)
{
	dynamicsWorld->setDebugDrawer(debugDrawer);	
}

void Physics::debugDraw()
{
	dynamicsWorld->debugDrawWorld();
}

void Physics::setDebugLevel(int level)
{
	dynamicsWorld->getDebugDrawer()->setDebugMode(level);
}

btDiscreteDynamicsWorld* Physics::getDiscreteDynamicsWorld(){
	return dynamicsWorld;
}

btVector3 Physics::getGravity(){return gravity;}

void Physics::updateVariables(ReloadEvent *e){
	setGravity(e->numberHolder.physicsInfo.gravity);
	staticFrictionScalar = e->numberHolder.physicsInfo.staticFrictionModifier;
	dynamicFrictionScalar = e->numberHolder.physicsInfo.dynamicFrictionModifier;	
}