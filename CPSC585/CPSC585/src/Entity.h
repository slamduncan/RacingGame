#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <string.h>
#include <sstream>

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btMatrix3x3.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "objLoader.h"

class Entity
{
private:
	bool isInit;
	int loaded;

	btScalar mass;

	bool init();

public:
	
	/*
	*	DEPRECIATED
	*	will use functions:
	*	getPosition();
	*	getTangent();
	*	getNormal();
	*	getBinormal();
	*
	*	These gets us all the data we need for a given entity
	*	since bullet will manage our entity position and orientation for us
	*/
	btVector3 position;
	btVector3 tangent;
	btVector3 normal;
	btVector3 binormal;

	// render model (objloader)
	// i believe there is a bug in the obj loader since texture coordinates are not being handled properly
	// NEED TO ASK PROF AND GROUP ABOUT THIS.
	objLoader* renderObject;

	// physics model (bullet)
	btRigidBody* physicsObject;


	Entity();
	//Entity(char* filename);
	Entity(char* filename, btScalar &mass, btTransform &trans, btVector3 &inertia);
	~Entity();

	void move(float x, float y, float z);
	void move(const btVector3 &newPos);
	void rotate(const btVector3 &axis, int deg);
	btScalar* getGLMatrix();

	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();

	bool loadObj(char* filename, btScalar &mass, btTransform &trans, btVector3 &inertia);

	void debug();
	std::string toString();
};

#endif