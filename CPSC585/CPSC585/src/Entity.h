#ifndef ENTITY_H
#define ENTITY_H

#include"Point.h"

class Entity
{
private:
	// center of mass
	Point position;

	// coordinate frame
	Point tangent;
	Point normal;
	Point binormal;

	// obj model (objloader)
	// NEED TO ASK PROF AND GROUP ABOUT THIS.

public:
	Entity();
	~Entity();
};

#endif