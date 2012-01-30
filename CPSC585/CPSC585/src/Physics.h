#ifndef PHYSICS_H
#define PHYSICS_H
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "Entity.h"

class Physics
{
public:
	~Physics(void);
	static Physics* Inst();

	void step();
	void setGravity(const btVector3 &gravity);
	void addEntity(const Entity &ent);
	void removeEntity(const Entity &ent);

	
protected:
	Physics(void);	//constructor is protected as it's a singleton

private:
	static Physics* physInstance;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif