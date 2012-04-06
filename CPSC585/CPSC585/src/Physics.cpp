#include "Physics.h"
#include "EntityManager.h"
#include "Renderer.h"
#include "PowerUp.h"
#include "Rocket.h"
#include "SlowField.h"
#include "Time.h"
#include "SoundPlayer.h"

Physics* Physics::physInstance = 0;
EntityManager* entityManager;
Renderer* physRender;

SoundPlayer soundEffectPlayer;

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
	//printf("Velocity: %f\n",entityManager->getCar(0)->chassis->getLinearVelocity().length());

	dynamicsWorld->stepSimulation(timeStep, 10);//1/60.f,10);

	updateCarSprings(timeStep);

	//Check Powerups for car collisions
	for (int i=0; i< entityManager->getPowerUpList()->size(); i++){
		PowerUp* p = entityManager->getPowerup(i);

		//Respawn the powerup if it is time
		if(p->timeToRespawn < clock() && p->timeToRespawn != 0){
			p->timeToRespawn = 0;
			p->setCollected(false);
		}

		//btCollisionObject* toDelObject = entityManager->getPowerup(i)->physicsObject;
		btGhostObject* go = btGhostObject::upcast(entityManager->getPowerup(i)->physicsObject);
		btAlignedObjectArray<btCollisionObject*> oa = go->getOverlappingPairs();
		for(int j=0; j< oa.size(); j++){
			btCollisionObject * carMaybe = oa.at(j);
			//Todo: check this pointer against all car pointers in carList
			int index = entityManager->getCarIndexViaPointer(carMaybe);
			if(index != -1)
			{
				if(p->isCollected() == false){
					p->setCollected(true);
					p->timeToRespawn = clock() + 3*CLOCKS_PER_SEC;

					Car* carTemp = entityManager->getCar(index);
					int added = carTemp->AddPowerUp(p->GetType());
				
					//dynamicsWorld->removeCollisionObject(entityManager->getPowerup(i)->physicsObject);
					//PowerUp* toDelete = entityManager->getPowerup(i);
					//entityManager->getPowerUpList()->remove(toDelete);
					//i--;
					
					break;
				}
			}
		}
	}



	for (int i = 0; i < entityManager->getSpawnableList()->size(); i++)
	{
		/* Deal with it if it's a rocket */
		Rocket* r = dynamic_cast<Rocket*>(entityManager->getSpawnableList()->at(i));
		if (r != NULL)
		{
			r->applyNextMove();
			btGhostObject* go = btGhostObject::upcast(r->physicsObject);
			btAlignedObjectArray<btCollisionObject*> oa = go->getOverlappingPairs();

			for(int j=0; j< oa.size(); j++){
				btCollisionObject * carMaybe = oa.at(j);
				//Todo: check this pointer against all car pointers in carList
				int index = entityManager->getCarIndexViaPointer(carMaybe);
				if(index != -1 && index != r->carId)
				{				
					Car* carTemp = entityManager->getCar(index);
					if (!carTemp->shieldActive)
						carTemp->chassis->applyTorque(r->getNormal()*500000.0);				
					
					dynamicsWorld->removeCollisionObject(r->physicsObject);
					entityManager->removeSpawnable(r);				
					i--;
					break;
				}			
			}
			continue;
		}
		/* Deal with it if it's a shield */		
		Shield* s = dynamic_cast<Shield*>(entityManager->getSpawnableList()->at(i));
		if (s != NULL)
		{
			s->updateLocation();
			continue;
		}
		//printf("RocketInd = %d\n", r->getNextWaypointIndex());

	}

	//Check SlowFields to see if they're removed or if they need to spawn anything
	for(int i=0; i< entityManager->getSlowFieldList()->size(); i++){
		SlowField * sf = entityManager->getSlowField(i);
		//printf("%d", sf->blobContainer->getNumChildShapes());
		
		if(sf->timeToDrop < clock()){
			//TODO: DROP ONE
			entityManager->createSlowFieldSpawnable("model/powerup.lwo", sf);

			sf->numSpawned++;
			if(sf->numSpawned > 9){
				//Set time to drop next way after the time the SlowField will self destruct
				sf->timeToDrop += 1000.f*CLOCKS_PER_SEC;
			}else{
				//Magic number! Set this as an XML variable later
				sf->timeToDrop += 0.10f*CLOCKS_PER_SEC;
			}
			
		}
		
		if(sf->timeToDelete < clock()){
			//Magic number! Set this as an XML variable later
			sf->timeToDelete += 0.10*CLOCKS_PER_SEC;

			//If ^ that is a magic number, then this is a whole bloody show.
			//Bad code to fix bullet's bad code. DON'T LOOK AT ME LIKE THAT
			switch(sf->numDeleted){
				case 0:
					sf->blobContainer->removeChildShapeByIndex(0);
					break;
				case 1:
					sf->blobContainer->removeChildShapeByIndex(1);
					break;
				case 2:
					sf->blobContainer->removeChildShapeByIndex(2);
					break;
				case 3:
					sf->blobContainer->removeChildShapeByIndex(3);
					break;
				case 4:
					sf->blobContainer->removeChildShapeByIndex(4);
					break;
				case 5:
					sf->blobContainer->removeChildShapeByIndex(5);
					break;
				case 6:
					sf->blobContainer->removeChildShapeByIndex(4);
					break;
				case 7:
					sf->blobContainer->removeChildShapeByIndex(3);
					break;
				case 8:
					sf->blobContainer->removeChildShapeByIndex(2);
					break;
				case 9:
					sf->blobContainer->removeChildShapeByIndex(1);
					break;
				case 10:
					entityManager->removeSlowField(sf);
					i--;
					break;
			}
			sf->numDeleted++;
			
		}
	}

	//Check slowfields to see if they need to slow cars or not
	for(int i=0; i < entityManager->getSlowFieldList()->size(); i++){
		SlowField* sf = entityManager->getSlowField(i);
		btGhostObject* go = btGhostObject::upcast(sf->physicsObject);
		btAlignedObjectArray<btCollisionObject*> oa = go->getOverlappingPairs();
		for(int j=0; j< oa.size(); j++){
			btCollisionObject * carMaybe = oa.at(j);
			//Todo: check this pointer against all car pointers in carList
			int index = entityManager->getCarIndexViaPointer(carMaybe);
			if(index != -1 && index != sf->carId)
			{
				//printf("YOU GOT ME A CAR!! OMG!!! OMG!!! \n");
				//printf("!");
				if (!entityManager->getCar(index)->shieldActive)
					entityManager->getCar(index)->setBeingSlowed();
			}
		}
	}

	//Check mines to see if they blow or not
	for(int i=0; i < entityManager->getMineList()->size(); i++){
		Mine* mine = entityManager->getMine(i);
		btGhostObject* go = btGhostObject::upcast(mine->physicsObject);
		btAlignedObjectArray<btCollisionObject*> oa = go->getOverlappingPairs();
		////--------------
		for(int j=0; j< oa.size(); j++){
			btCollisionObject * carMaybe = oa.at(j);
			//Todo: check this pointer against all car pointers in carList
			int index = entityManager->getCarIndexViaPointer(carMaybe);
			entityManager->getCar(index)->chassis->applyCentralForce(btVector3(0,15000.0,0));

			dynamicsWorld->removeCollisionObject(entityManager->getMine(i)->physicsObject);
			entityManager->removeMine(entityManager->getMine(i));
			i--;

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