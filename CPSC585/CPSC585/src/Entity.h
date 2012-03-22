#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btMatrix3x3.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "MethodObserver.h"


//#include "assimp.hpp"	//c++ interface
#include "assimp.h"	//c interface
#include "aiPostProcess.h"
#include "aiScene.h"

class Entity
{
private:
	//btTransform transform;


//	bool isInit;
//	int loaded;

//	bool init();

public:
	// pointer to an array of 16 elements
	// might as well store the matrix per frame? since that way we only have to
	// generate the array once per object instead of once per object per frame.
	btScalar* glMatrix;
	//btScalar mass;




	// render model (objloader)
	const aiScene* renderObject;

	// physics model (bullet)
	//btRigidBody* physicsObject;
	btCollisionObject* physicsObject;

	//Observer
	virtual void initObservers() = 0;
	


	Entity();
	//Entity(char* filename);
	//Entity(char* filename, btScalar &mass, btTransform &trans);
	virtual ~Entity();

	void move(float x, float y, float z);
	void move(const btVector3 &newPos);
	void rotate(const btVector3 &axis, int deg);
	
	virtual btScalar* getGLMatrix();

	virtual btVector3 getPosition();
	virtual btVector3 getTangent();
	virtual btVector3 getNormal();
	virtual btVector3 getBinormal();
	

	//bool loadObj(char* filename, btScalar &mass, btTransform &trans);
	
	bool initRenderObject(char* filename);
	virtual bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans) = 0;

	void debug();
	virtual std::string toString();
};

#endif