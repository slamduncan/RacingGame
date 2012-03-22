#include "Physics.h"
#include "EntityManager.h"
#include "Renderer.h"
#include "PowerUp.h"
#include "Time.h"

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

	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	//btInternalTickCallback cb = updateCarSprings;
	dynamicsWorld->setInternalTickCallback(myTickCallback, static_cast<void *>(this));
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

void Physics::step(btScalar &timeStep)
{	
	dynamicsWorld->stepSimulation(timeStep, 10);//1/60.f,10);

	updateCarSprings(timeStep);

	//Check Powerups for car collisions
	for (int i=0; i< entityManager->getPowerUpList()->size(); i++){
		//btCollisionObject* toDelObject = entityManager->getPowerup(i)->physicsObject;
		btGhostObject* go = btGhostObject::upcast(entityManager->getPowerup(i)->physicsObject);
		btAlignedObjectArray<btCollisionObject*> oa = go->getOverlappingPairs();
		for(int j=0; j< oa.size(); j++){
			btCollisionObject * carMaybe = oa.at(j);
			//Todo: check this pointer against all car pointers in carList
			int index = entityManager->getCarIndexViaPointer(carMaybe);
			if(index != -1)
			{
				
				Car* carTemp = entityManager->getCar(index);

				int added = carTemp->AddPowerUp(entityManager->getPowerup(i)->GetType());
				
				dynamicsWorld->removeCollisionObject(entityManager->getPowerup(i)->physicsObject);
				PowerUp* toDelete = entityManager->getPowerup(i);
				entityManager->getPowerUpList()->remove(toDelete);
				i--;
				break;
			}
		}
	}

	//Check SlowFields to see if they're removed or if they need to spawn anything
	for(int i=0; i< entityManager->getSlowFieldList()->size(); i++){
		SlowField * sf = entityManager->getSlowField(i);
		if(sf->timeToSelfDestruct < clock()){
			entityManager->removeSlowField(sf);
			i--;
		}else if(sf->timeToDrop < clock()){
			//TODO: DROP ONE
			entityManager->createSlowFieldSpawnable("model/powerup.lwo", sf);

			sf->numSpawned++;
			if(sf->numSpawned > 5){
				//Set time to drop next way after the time the SlowField will self destruct
				sf->timeToDrop += 1000*CLOCKS_PER_SEC;
			}else{
				sf->timeToDrop += 1*CLOCKS_PER_SEC;
			}
		}
	}

	//Check spawnables to see if they're removed or not
	for(int i=0; i< entityManager->getSpawnableList()->size(); i++){
		Spawnable * s = entityManager->getSpawnable(i);
		if(s->timeToSelfDestruct < clock() && s->timeToSelfDestruct != 0){
			entityManager->removeSpawnable(s);
			i--;
		}
	}
}

void Physics::updateCarSprings(btScalar timeStep)
{
	for (int i = 0; i < entityManager->getCarList()->size(); i++){
		//update springs
		entityManager->getCarList()->at(i)->updateWheels();	
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

void Physics::addGhost(btGhostObject * ghost)
{
	dynamicsWorld->addCollisionObject(ghost);
}

void Physics::removeGhost(btGhostObject * ghost)
{
	dynamicsWorld->removeCollisionObject(ghost);
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