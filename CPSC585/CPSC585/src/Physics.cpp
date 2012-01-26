#include "Physics.h"

//HIIIIII
Physics* Physics::physInstance = 0;

Physics* Physics::Inst(void){	
	if(physInstance == 0){
		physInstance = new Physics();
	}
	return physInstance;
}

Physics::Physics(void)
{
	/*
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	*/

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
}

Physics::~Physics(void)
{
	delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void Physics::step()
{

}