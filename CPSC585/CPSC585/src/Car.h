#ifndef CAR_H
#define CAR_H

#include <assert.h>

#include "TypedObserver.h"
#include "TriggerEvent.h"
#include "RotationEvent.h"
#include "ForwardForceEvent.h"
#include "Entity.h"
#include "Spring.h"
#include "Wheel.h"
#include "PowerUp.h"

#ifndef MAX_POWERUPS
#define MAX_POWERUPS           3 // Use this value when initializing array of power-ups
#endif

class Car : public Entity
{

public:
	btRigidBody* chassis;
	
	btScalar kVal;
	
	int resetCounter;
	int AIresetCounter;
	//Variable added to test the new spring system.
	btScalar kValue;
	btScalar critDampingValue;
	btScalar hoverValue;
	btScalar carMass;
	btScalar restDisplacement;
	btVector3 gravity;
	btScalar lastAngleForce;
	int id;

	Car();
	bool initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans);

	Wheel newWheels[4];

	Spring wheels[4];
	btVector3 wheelOffsets[4];
	void updateWheels();
	void updateSpringLocations();

	void initObservers();

	void observeRotation(RotationEvent *e);
	void observeForwardForce(ForwardForceEvent *e);
	void observeVariables(ReloadEvent *e);

	void setUpWheelStuff();

	int getNextWaypointIndex();
	void setNextWaypointIndex(int in);

	PowerUp * GetPowerUpAt( int index );
	
	int AddPowerUp( int type );
	int GetNumberPowerUps();
	void UsePowerUp( int index, bool offensive );

	void RotatePowerups( bool RotateLeft );

	float GetSpeed();
	void SetSpeed( float speed );
	btCollisionObject* getPhysicsObject();

	float GetForwardForceModifier();

	std::string toString();

	void outputPowerups();

	//Pass in true if the closest car found must be infront of the other car.
	//Returns NULL if no car is found in range.
	Car* getClosestCar(bool inFront);
	Car* getClosestCar(bool inFront, float detectionDistance);
	void setBeingSlowed();

	btCompoundShape * slowBlobContainer;
	bool shieldActive;
	bool halfWayAround; //For lap counts.
	int lapCount;
	bool usedPowerUpRecently;
	std::stringstream timeFinished;
	bool finishedRacing;
	int finalPosition;

private:
	btScalar width, length, height;
	int nextWaypoint;
	float m_Speed;
	float forwardForceModifier;
	float sideFrictionModifier;
	float forwardFrictionModifier;
	float turningForceModifier;
	float springForceModifier;
	float slidingFrictionModifier;
	
	float speedBoostModifier; 
    float slowFieldModifier;
    float rocketModifier;
    float tractionBoostModifier;
    float shieldModifier;
    float forceBubbleModifier;
	float detectionRange;

	bool beingSlowed;

	// 3 power up slots, circular array of powerups?
	//btAlignedObjectArray<PowerUp*> m_CarPowerUps;
	PowerUp m_CarPowerUps[MAX_POWERUPS];


	//Observers
	MethodObserver<RotationEvent, Car> rotationObserver;
	MethodObserver<ForwardForceEvent, Car> forwardForceObserver;
	MethodObserver<ReloadEvent, Car> updateVariableObserver;
};
#endif