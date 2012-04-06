#include "Effect.h"

Effect::Effect()
{
	transform = btTransform(btQuaternion(0, 1, 0, 1), btVector3(0, 0, 0));
	ttl = btScalar(2000.f);
	timer.start();
	this->type = 0;
	scale = btScalar(1.f);
}

Effect::Effect(btScalar ttl, btTransform& trans, int type)
{
	this->ttl = ttl;
	transform = trans;
	timer.start();
	this->type = type;
	scale = btScalar(1.f);
}

Effect::~Effect()
{
	
}

void Effect::initObservers()
{

}

bool Effect::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	transform = trans;

	return true;
}

btScalar* Effect::getGLMatrix()
{
	transform.getOpenGLMatrix(glMatrix);

	return glMatrix;
}
btVector3 Effect::getPosition()
{
	return transform.getOrigin();
}
btVector3 Effect::getTangent()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 tan = orientation.getColumn(0);

	return tan;
}
btVector3 Effect::getNormal()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 nor = orientation.getColumn(1);

	return nor;
}
btVector3 Effect::getBinormal()
{
	btMatrix3x3 orientation = transform.getBasis();

	btVector3 bin = orientation.getColumn(2);

	return bin;
}

void Effect::startTimer()
{
	timer.start();
}

void Effect::stopTimer()
{
	timer.stop();
}

bool Effect::timeUp()
{
	btScalar tMill = btScalar(timer.get_ticks());

	if(tMill >= ttl)
	{
		return true;
	}
	return false;
}

int Effect::getType()
{
	return type;
}