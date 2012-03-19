#ifndef PHYSICS_H
#define PHYSICS_H
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"
#include "Entity.h"
#include "ReloadEvent.h"
#include "MethodObserver.h"
#include "EventTypes.h"
static void myTickCallback(btDynamicsWorld *world, btScalar timeStep);
class Physics
{
public:
	~Physics(void);
	static Physics* Inst();

	void step(btScalar &timeStep);
	void setGravity(const btVector3 &gravity);
	void addEntity(const Entity &ent);
	void removeEntity(const Entity &ent);
	
	void addRigidBody(const Entity &entity);
	void removeRigidBody(const Entity &entity);

	void setDebugDrawer(btIDebugDraw *debugDrawer);
	void debugDraw();
	void setDebugLevel(int level);
	btDiscreteDynamicsWorld* getDiscreteDynamicsWorld();
	btVector3 getGravity();
	void updateVariables(ReloadEvent *e);
	void updateCarSprings(btScalar timeStep);
	void addGhost(btGhostObject * ghost);
	void removeGhost(btGhostObject * ghost);
	
protected:
	Physics(void);	//constructor is protected as it's a singleton

private:
	static Physics* physInstance;

	btVector3 gravity;
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btScalar staticFrictionScalar, dynamicFrictionScalar;

	MethodObserver<ReloadEvent, Physics> variableObserver;
};

void myTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    Physics *w = static_cast<Physics *>(world->getWorldUserInfo());
    
	//printf("WHAT HAS SCIENCE DONE!\n");
	w->updateCarSprings(timeStep);
}

#endif