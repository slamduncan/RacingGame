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
//#include "objLoader.h"
#include "MethodObserver.h"
#include "RotationEvent.h"
#include "ForwardForceEvent.h"

//TESTING ASSIMP for animated based models
//#include "assimp.hpp"	//c++ interface
#include "assimp.h"	//c interface
#include "aiPostProcess.h"
#include "aiScene.h"

class Entity
{
private:
	bool isInit;
	int loaded;

	btScalar mass;

	bool init();

	//Observers
	MethodObserver<RotationEvent, Entity> rotationObserver;
	MethodObserver<ForwardForceEvent, Entity> forwardForceObserver;

public:
	// pointer to an array of 16 elements
	// might as well store the matrix per frame? since that way we only have to
	// generate the array once per object instead of once per object per frame.
	btScalar* glMatrix;

	// render model (objloader)
	const aiScene* scene;

	// physics model (bullet)
	btRigidBody* physicsObject;



	//Observer
	void initObservers();

	void observeRotation(RotationEvent *e){		
//		physicsObject->setAngularFactor(1000);
		
		btVector3 test = e->getQuaternion().getAxis();
		btVector3 temp = physicsObject->getAngularVelocity();
		if (temp.length() < 5)
		//physicsObject->setAngularVelocity(test);
			physicsObject->applyTorque(test);		
	};

	void observeForwardForce(ForwardForceEvent *e){
		btVector3 tan = getTangent() * (e->getNormForce());

		physicsObject->applyCentralImpulse(tan);
	}
	


	Entity();
	//Entity(char* filename);
	Entity(char* filename, btScalar &mass, btTransform &trans);
	~Entity();

	void move(float x, float y, float z);
	void move(const btVector3 &newPos);
	void rotate(const btVector3 &axis, int deg);
	btScalar* getGLMatrix();

	btVector3 getPosition();
	btVector3 getTangent();
	btVector3 getNormal();
	btVector3 getBinormal();

	bool loadObj(char* filename, btScalar &mass, btTransform &trans);

	void debug();
	std::string toString();
};

#endif