#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>

#include "../include/bullet/btVector3.h"
#include "../include/objLoader/objLoader.h"

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

	bool loadObj(char* filename);
};

#endif