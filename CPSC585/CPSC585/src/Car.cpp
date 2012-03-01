#include "Car.h"
#include "EventSystemHandler.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

//
//	Current Car representation
//
//                        ^ z
//                        |
//                        |
//            |-----------+------------|
//     wheel3 |           |            | wheel 2
//            |           |            |
//            |           |            |
//            |           |            |
//            |           |            |
//         x  |           |            |
//        <---+-----------.(y out of   |
//            |             screen)    |
//            |                        |
//            |                        |
//            |                        |
//            |                        |
//     wheel1 |                        | wheel 0
//            |------------------------|
Car::Car() : rotationObserver(this, &Car::observeRotation), 
			forwardForceObserver(this, &Car::observeForwardForce),
			updateVariableObserver(this, &Car::observeVariables)
{		
	Car::width = btScalar(5.0f);
	Car::height = btScalar(5.0f);
	Car::length = btScalar(10.0f);
	gravity = Physics::Inst()->getGravity();
	restDisplacement = btScalar(2.0f);
	nextWaypoint = 0;
	m_Speed = 0.0f;
	forwardForceModifier = 1.0;
	sideFrictionModifier = 1.0;
	forwardFrictionModifier = 1.0;
	updateVariableObserver.init(EventTypes::RELOAD_VARIABLES);
}

void Car::initObservers()
{	
	rotationObserver.init(EventTypes::ROTATION);
	forwardForceObserver.init(EventTypes::FORWARD_FORCE);	
}

void Car::observeRotation(RotationEvent *e){		
	btVector3 test = e->getQuaternion().getAxis();
	
	btVector3 temp = chassis->getAngularVelocity();
	//btVector3 temp = physicsObject->getAngularVelocity();
	
	if (temp.length() < 5)
	//physicsObject->setAngularVelocity(test);
		//physicsObject->applyTorque(test);
		chassis->applyTorque(test);	
}

void Car::observeForwardForce(ForwardForceEvent *e){
	
	btScalar engineForce = e->getNormForce();
	
	btVector3 tan = getTangent() * engineForce * forwardForceModifier;
	tan.setY(0);	// project to the xz plane
	tan /= 4.0f;
	if(engineForce < 0)
	{
		float NewSpeed = m_Speed + (engineForce * -0.0005);
		if( NewSpeed < (engineForce * -1.0) )
			m_Speed = NewSpeed;

		// player is accelerating, we apply rear wheel force
		if(newWheels[2].onGround || newWheels[3].onGround)
		{
			chassis->applyForce(tan, wheelOffsets[2]);
			chassis->applyForce(tan, wheelOffsets[3]);
		}
	}
	// player is decelerating
	else
	{
		float NewSpeed = m_Speed - 0.0005;
		if( NewSpeed > 0 )
			m_Speed = NewSpeed;

		// apply to all the wheels
		for(int i = 0; i < 4; i++)
		{
			if(newWheels[i].onGround)
			{
				chassis->applyForce(tan, wheelOffsets[i]);
			}
		}
	}
}

/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Car::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	kVal = (mass * 10.0f)/(3.0f * 4.0f);
	if(cShape != NULL)
	{
		btVector3 inertia;
		
		cShape->calculateLocalInertia(mass, inertia);
		
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,cShape,inertia);

		physicsObject = new btRigidBody(entRigidBodyCI);

		chassis = btRigidBody::upcast(physicsObject);

		physicsObject->setActivationState(DISABLE_DEACTIVATION);
		chassis->setActivationState(DISABLE_DEACTIVATION);

		updateSpringLocations();
		setUpWheelStuff();
		btScalar wheelLength(4.0f);
	
		newWheels[0] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue, (gravity),(getPosition() + wheelOffsets[0]),
			(getPosition() + wheelOffsets[0] - getNormal()*3.0f), chassis);
		newWheels[1] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue, gravity, getPosition() + wheelOffsets[1],
			getPosition() + wheelOffsets[1] - getNormal()*3.0f, chassis);
		newWheels[2] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue,critDampingValue, gravity, getPosition() + wheelOffsets[2],
			getPosition() + wheelOffsets[2] - getNormal()*3.0f, chassis);
		newWheels[3] = Wheel(hoverValue, wheelLength, btScalar(3),
			kValue, critDampingValue,gravity, getPosition() + wheelOffsets[3],
			getPosition() + wheelOffsets[3] - getNormal()*3.0f, chassis);			

		return true;
	}

	return false;
}

void Car::updateWheels()
{
	updateSpringLocations();

	// simulate suspension
	btVector3 forces[4];
	btScalar sideFriction[4] = {btScalar(0.f),btScalar(0.f),btScalar(0.f),btScalar(0.f)}; 
	for (int i = 0; i < 4; i++){
		forces[i] = newWheels[i].calcForce(getPosition() + wheelOffsets[i], getNormal());
	}

	// simulate side friction
	for(int i = 0; i < 4; i++)
	{
		if(newWheels[i].hitObject)
		{
			btVector3 contact = newWheels[i].getBottomSpringPosition();
			
			btRigidBody* groundObject = (class btRigidBody*) newWheels[i].hitObject;
			
			resolveSingleBilateral(*chassis, contact, *groundObject, contact, btScalar(0.),getBinormal(), sideFriction[i], 1/60.0f);
		}
	}

	for (int i = 0; i < 4; i++){
		btVector3 contact = newWheels[i].getBottomSpringPosition();
		chassis->applyImpulse(forces[i],contact - chassis->getCenterOfMassPosition()/*wheelOffsets[i]*/);
		
		if(sideFriction[i] != btScalar(1.))
		{
			btVector3 carNormal = getNormal();
			
			btVector3 relpos = contact - chassis->getCenterOfMassPosition();

			relpos -= carNormal * (carNormal.dot(relpos));

			chassis->applyForce(getBinormal() * sideFriction[i]*0.1f * sideFrictionModifier,relpos);
		}
	}
}

// probably not needed anymore
void Car::updateSpringLocations()
{
	btVector3 position = getPosition();
	btVector3 tangent = getTangent();
	btVector3 normal = getNormal();
	btVector3 binormal = getBinormal();

	wheelOffsets[0] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) + 0.5f/* + 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[1] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) - 0.5f/* - 0.5f*/)) + (tangent * (-length/2.0f + 1.0f));
	wheelOffsets[2] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((-width/2.0f) + 0.5f/* + 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
	wheelOffsets[3] = (normal * ((-height/2.0f) + 0.5f)) + (binormal * ((+width/2.0f) - 0.5f/* - 0.5f*/)) + (tangent * (+length/2.0f - 1.0f));
}

void Car::setUpWheelStuff()
{
	kValue = 20.0f;
	critDampingValue = 2 * btSqrt(kValue * 2.0f);
	hoverValue = btScalar(1.0f);
}

/* This function allows for dynamic modification of the c and k values for the wheels. */
void Car::observeVariables(ReloadEvent *e){
	for (int i = 0; i < 4; i++){
		newWheels[i].setCModifier(btScalar(e->numberHolder.physicsInfo.cModifier));
		newWheels[i].setKModifier(btScalar(e->numberHolder.physicsInfo.kModifier));
	}
	forwardForceModifier = e->numberHolder.physicsInfo.forwardForceModifier;
	sideFrictionModifier = e->numberHolder.physicsInfo.sideFrictionModifier;
	forwardFrictionModifier = e->numberHolder.physicsInfo.forwardFrictionModifier;
}

PowerUp Car::GetPowerUpAt( int index )
{
	if( index >= 0 && index < MAX_POWERUPS  )
		return m_CarPowerUps[index];
}

void Car::AddPowerUp( int type )
{
	for( int i = 0; i < MAX_POWERUPS; i++ )
	{
		if( m_CarPowerUps[i].GetType() == EMPTY )
		{
			m_CarPowerUps[i].SetType( type );
			break;
		}
	}
}

void Car::UsePowerUp( int index )
{
	if( index >= 0 && index < MAX_POWERUPS  )
		m_CarPowerUps[index].SetType( EMPTY );
}

int Car::getNextWaypointIndex(){return nextWaypoint;}
void Car::setNextWaypointIndex(int in){ nextWaypoint = in;}

float Car::GetSpeed()
{
	return m_Speed;
}

void Car::SetSpeed( float speed )
{
	m_Speed = speed;
}