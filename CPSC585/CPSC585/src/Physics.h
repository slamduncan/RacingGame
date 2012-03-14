#ifndef PHYSICS_H
#define PHYSICS_H
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"
#include "Entity.h"
#include "ReloadEvent.h"
#include "MethodObserver.h"
#include "EventTypes.h"

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
	void updateCarSprings();

	
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

#endif