#pragma once

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"

class Light
{
private:
	btVector3 position;

public:

	double modelView[16];
	double projection[16];

	Light(void);
	~Light(void);
	Light(btVector3 &initPos);

	btVector3 getPosition();
	void setPosition(btVector3 &pos);
};
