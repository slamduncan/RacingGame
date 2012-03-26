#ifndef SKYSPHERE_H
#define SKYSPHERE_H

#include "Entity.h"

class SkySphere : public Entity
{
public:
	SkySphere();
	~SkySphere();

	void initObservers();
	bool initPhysicsObject(btCollisionShape * cShape, btScalar &mass, btTransform &trans);
	

	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();
	btTransform getTransform();
	void setTransform(btTransform &inT);
	btScalar* getGLMatrix();

private:
	btTransform transform;
};

#endif