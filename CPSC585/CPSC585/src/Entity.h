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
#include "objLoader.h"

class Entity
{
private:
	bool isInit;
	int loaded;

	// physicsObject

	bool init();

public:
		// center of mass
	btVector3 position;

	// coordinate frame
	btVector3 tangent;
	btVector3 normal;
	btVector3 binormal;

	btMatrix3x3 cf;

	// obj model (objloader)
	// NEED TO ASK PROF AND GROUP ABOUT THIS.
	objLoader* renderObject;

	// physics model (bullet)
	btBvhTriangleMeshShape* physicsObject;


	Entity();
	Entity(char* filename);
	~Entity();

	void move(float x, float y, float z);
	void move(const btVector3 &newPos);
	void rotate(const btVector3 &axis, int deg);
	
	bool loadObj(char* filename);
	void debug();
	std::string toString();
};

#endif