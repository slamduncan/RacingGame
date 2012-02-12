#include "Physics.h"
#include "EntityManager.h"
#include "Renderer.h"

//HIIIIII
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
	/*
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	*/
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

void Physics::step()
{	
	//physRender->updateGL();
 	
	dynamicsWorld->stepSimulation(1/60.f,10);

	for (int i = 0; i < entityManager->getCarList()->size(); i++){
		entityManager->getCarList()->at(i)->updateWheels();
		//entityManager->getCarList()->at(i)->cheatAndFixRotation();
	}

	//btCollisionObjectArray temp = dynamicsWorld->getCollisionObjectArray();

	//btCollisionObject* temp2 = temp.at(0);

	//btTransform trans = temp2->getWorldTransform();

	

	//printf("car data: %f,%f,%f\n", trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()); 
}

void Physics::setGravity(const btVector3 &gravityIn)
{
	gravity = gravityIn;
	dynamicsWorld->setGravity(gravityIn);
}

void Physics::addEntity(const Entity &ent)
{
	dynamicsWorld->addRigidBody(ent.physicsObject);
}

void Physics::removeEntity(const Entity &ent)
{
	dynamicsWorld->removeRigidBody(ent.physicsObject);
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
	setGravity(e->numberHolder.gravity);
}