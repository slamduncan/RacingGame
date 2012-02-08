#ifndef TRACK_H
#define TRACK_H
#include "Entity.h"

class Track : public Entity
{
public:
	Track();
	~Track();

	//bool initRenderObject(char* filename);
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);
	void initObservers();
};

#endif
