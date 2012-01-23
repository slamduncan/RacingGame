#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <string.h>
#include <sstream>

#include "LinearMath/btVector3.h"
#include "objLoader.h"

class Entity
{
private:
	bool isInit;

	// physicsObject

	bool init();

public:
		// center of mass
	btVector3 position;

	// coordinate frame
	btVector3 tangent;
	btVector3 normal;
	btVector3 binormal;

	// obj model (objloader)
	// NEED TO ASK PROF AND GROUP ABOUT THIS.
	objLoader* renderObject;


	Entity();
	Entity(char* filename);
	~Entity();

	void move(float x, float y, float z);
	void move(const btVector3 &newPos);
	bool loadObj(char* filename);
	void debug();
	std::string toString();
};

#endif