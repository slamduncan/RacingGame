#include "Car.h"
#include "EventSystemHandler.h"
#include "EntityManager.h"

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
	nextWaypoint = -1;
	turningForceModifier = 1.0;
	springForceModifier = 1.0;
	lastAngleForce = 0.0;
	resetCounter = 0;
	speedBoostModifier = 1; 
	slowFieldModifier = 1;
	rocketModifier = 1;
	tractionBoostModifier = 1;
	shieldModifier = 1;
	forceBubbleModifier = 1;
	detectionRange = 10.0;
	slidingFrictionModifier = 1.0;

	PowerUp p1 = PowerUp();
	PowerUp p2 = PowerUp();
	PowerUp p3 = PowerUp();
	shieldActive = false;
	usedPowerUpRecently = false;

	m_CarPowerUps[0] = p1;
	m_CarPowerUps[1] = p2;
	m_CarPowerUps[2] = p3;

	beingSlowed = false;
	halfWayAround = false;
	lapCount = 2;
	timeFinished;
	finishedRacing = false;
	finalPosition = -1;
}

void Car::initObservers()
{	
	rotationObserver.init(EventTypes::ROTATION);
	forwardForceObserver.init(EventTypes::FORWARD_FORCE);	
}

void Car::observeRotation(RotationEvent *e){		
	btVector3 test = e->getQuaternion().getAxis()*turningForceModifier;

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

	// Calculate the artificial speed of the car to be used
	// for engine sound
	float NewSpeed = GetSpeed() + (engineForce * -0.0005f);
	if( NewSpeed < (engineForce * -1.0f) )
		SetSpeed(NewSpeed);

	if(engineForce < 0)
	{
		//if car is in a slow field, we cap linear velocity at 100
		if(beingSlowed == false || chassis->getLinearVelocity().length() < slowFieldModifier){
			// player is accelerating, we apply rear wheel force
			if(newWheels[2].onGround || newWheels[3].onGround)
			{
				chassis->applyForce(tan, wheelOffsets[2]);
				chassis->applyForce(tan, wheelOffsets[3]);
			}
		}else{
			beingSlowed = false;
		}
	}
	// player is decelerating
	else
	{				
		// apply to all the wheels
		for(int i = 0; i < 4; i++)
		{
			
			if(newWheels[i].onGround)
			{
				//Apply Reverse Cap
				if (tan.length() > 100)
					tan = tan.normalize() * 100.0;
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
	btScalar forwardFriction[4] = {btScalar(0.f),btScalar(0.f),btScalar(0.f),btScalar(0.f)};
	for (int i = 0; i < 4; i++){
		forces[i] = newWheels[i].calcForce(getPosition() + wheelOffsets[i], getNormal());
	}

	// simulate side friction, forward friction
	//printf("LINEAR = %f, %f, %f\n" , chassis->getLinearVelocity().x(), chassis->getLinearVelocity().y(), chassis->getLinearVelocity().z());
	//printf("DeltaLinear = %f, %f, %f\n" , chassis->getDeltaLinearVelocity().x(), chassis->getDeltaLinearVelocity().y(), chassis->getDeltaLinearVelocity().z());
	btVector3 sideFricMag = -(this->chassis->getAngularVelocity());
	//printf("Angular = %f, %f, %f\n" , chassis->getAngularVelocity().x(), chassis->getAngularVelocity().y(), chassis->getAngularVelocity().z());
	//printf("DeltatAngle = %f, %f, %f\n" , chassis->getDeltaAngularVelocity().x(), chassis->getDeltaAngularVelocity().y(), chassis->getDeltaAngularVelocity().z());

	btVector3 forwardFricMag = -(this->chassis->getLinearVelocity());
	for(int i = 0; i < 4; i++)
	{
		if(newWheels[i].hitObject)
		{			
			btVector3 contact = newWheels[i].getBottomSpringPosition();

			btRigidBody* groundObject = (class btRigidBody*) newWheels[i].hitObject;

			//resolveSingleBilateral(*chassis, contact, *groundObject, contact, btScalar(0.),getBinormal(), sideFriction[i], 1/60.0f);
			//sideFriction[i] = -sideFricMag/4 * sideFrictionModifier *0.05 ;
			//printf("%f\t", sideFriction[i]);
			//sideFriction[i] *=sideFrictionModifier;

			//resolveSingleBilateral(*chassis, contact, *groundObject, contact, btScalar(0.),getTangent(), forwardFriction[i], 1/60.0f);
			//forwardFriction[i] = -forwardFricMag * forwardFrictionModifier * 0.1;
			//forwardFriction[i] *=forwardFrictionModifier;
		}
	}
	//printf("\n");


	for (int i = 0; i < 4; i++){
		btVector3 contact = newWheels[i].getBottomSpringPosition();

		chassis->applyForce(forces[i]*springForceModifier,contact - chassis->getCenterOfMassPosition()/*wheelOffsets[i]*/);
		if(sideFriction[i] != btScalar(1.) && newWheels[i].hitObject)
		{
			/*
			btVector3 carNormal = getNormal();

			btVector3 relpos = contact - chassis->getCenterOfMassPosition();

			relpos -= carNormal * (carNormal.dot(relpos));

			chassis->applyForce(getBinormal() * sideFriction[i]*0.1f * sideFrictionModifier,relpos);
			*/
			chassis->applyTorque(sideFricMag/4.0f * sideFrictionModifier);
			
		}

		if(forwardFriction[i] != btScalar(1.) && newWheels[i].hitObject)
		{
			/*
			btVector3 carNormal = getNormal();

			btVector3 relpos = contact - chassis->getCenterOfMassPosition();

			relpos -= carNormal * (carNormal.dot(relpos));

			chassis->applyForce(getTangent() * forwardFriction[i]*0.1f * forwardFrictionModifier,relpos);
			*/
			chassis->applyCentralForce(forwardFricMag * forwardFrictionModifier);

			/* Apply sideways friction so it less like ice driving... */
			float amountOnBi = forwardFricMag.dot(getBinormal());
			btScalar forceAmount(0);
			if (amountOnBi < 0)
				forceAmount = -sqrt(abs(forwardFricMag.dot(getBinormal()))) * slidingFrictionModifier;
			else
				forceAmount = sqrt(abs(forwardFricMag.dot(getBinormal()))) * slidingFrictionModifier;
			chassis->applyCentralForce(getBinormal() * forceAmount);
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
	turningForceModifier = e->numberHolder.physicsInfo.turningForceModifier;
	springForceModifier = e->numberHolder.physicsInfo.springForceModifier;

	speedBoostModifier = e->numberHolder.physicsInfo.speedBoostModifier;
	slowFieldModifier = e->numberHolder.physicsInfo.slowFieldModifier;
	rocketModifier = e->numberHolder.physicsInfo.rocketModifier;
	tractionBoostModifier = e->numberHolder.physicsInfo.tractionBoostModifier;
	shieldModifier = e->numberHolder.physicsInfo.shieldModifier;
	forceBubbleModifier = e->numberHolder.physicsInfo.forceBubbleModifier;
	detectionRange = e->numberHolder.aiInfo.carDetectionRange;
	slidingFrictionModifier = e->numberHolder.physicsInfo.slidingFrictionModifier;
}

PowerUp * Car::GetPowerUpAt( int index )
{
	assert(index >= 0 && index < MAX_POWERUPS);

	return & m_CarPowerUps[index];
}

int Car::AddPowerUp( int type )
{
	//	printf("NP Before: %d\n", GetNumberPowerUps());

	for( int i = 0; i < MAX_POWERUPS; i++ )
	{
		if( m_CarPowerUps[i].GetType() == EMPTY )
		{
			//printf("Adding powerup at location %d with type %d\n",i,type);
			m_CarPowerUps[i].SetType( type );
			//			printf("NP After: %d\n", GetNumberPowerUps());
			return 1;
		}
	}
	return 0;
}

int Car::GetNumberPowerUps(){

	int count = 0;
	for(int i = 0; i < MAX_POWERUPS; i++){
		if(m_CarPowerUps[i].GetType() != EMPTY){
			count++;
		}
	}
	return count;
}

void Car::UsePowerUp( int index , bool offensive)
{
	if( index >= 0 && index < MAX_POWERUPS ){
		int pUpType = m_CarPowerUps[index].GetType();
		m_CarPowerUps[index].SetType( EMPTY );
		usedPowerUpRecently = true;
		EntityManager * ent = EntityManager::getInstance();
		//printf("Activating powerup with type %i!\n",pUpType);
		switch(pUpType){
			case SPEED_SLOW:{
				//SPEED POWERUP
				if (!offensive)
				{
					btVector3 forward = getTangent();
					forward.setY(0);
					chassis->applyCentralForce(-speedBoostModifier*forward);
				}
				//SLOW POWERUP
				else {
					EntityManager * ent;
					ent = EntityManager::getInstance();
					ent->createSlowField(this);				
				}
				break;
							}
			case ROCKET_SHIELD:
				{
					//ROCKET POWERUP
					if (offensive)
					{
						btTransform rocketT= physicsObject->getWorldTransform();				
						rocketT.setOrigin( rocketT.getOrigin() - getTangent()*8.0);
						ent->createRocket(this->getNextWaypointIndex(), rocketT, id);
						//ent->createCar("model/ship1.lwo", carMass, rocketT);
					}
					//Shield PowerUP
					else{
						ent->createShield(this);
					}					
					break;
				}
			case NOVA_MINE:
				{
					//NOVA POWERUP
					if (!offensive)
					{
						btGhostObject * explosionShell;
						explosionShell = new btGhostObject();
						btCollisionShape* sphereShape;
						sphereShape = new btSphereShape(btScalar(forceBubbleModifier));
						explosionShell->setCollisionShape(sphereShape);
						explosionShell->setWorldTransform(physicsObject->getWorldTransform());
						explosionShell->setCollisionFlags(physicsObject->CF_NO_CONTACT_RESPONSE);

						Physics * phys = Physics::Inst();				
						phys->addGhost(explosionShell);

						btAlignedObjectArray<btCollisionObject*> oa = explosionShell->getOverlappingPairs();
						//printf("I hit %i things!\n",oa.size());

						for(int i=0; i< oa.size(); i++){
							btCollisionObject * carMaybe = oa.at(i);

							int index = ent->getCarIndexViaPointer(carMaybe);
							if(index != -1){
								Car* carTemp = ent->getCar(index);
								if(carTemp != this){
									//APPLY NOVA FORCE TO THE CAR! BAM!
									btScalar modifier = 400.f -(carTemp->getPosition().distance(getPosition()));
									//printf("Modifier: %f\n",modifier);

									btVector3 between = carTemp->getPosition()-getPosition();
									btVector3 force = between.normalized();

									force = force*forceBubbleModifier;
									force = force - between;
									force = modifier*force;

									carTemp->chassis->applyCentralForce(force);
								}
							}
						}				
						phys->removeGhost(explosionShell);
					}
					//MINE POWERUP
					else 
					{
						EntityManager * ent;
						ent = EntityManager::getInstance();
						ent->createMine(this,"model/waypoint.obj");
					}
					break;
				}
		}
	}
}

int Car::getNextWaypointIndex(){return nextWaypoint;}
void Car::setNextWaypointIndex(int in){ nextWaypoint = in;}

float Car::GetSpeed()
{
	float temp1 = chassis->getLinearVelocity().dot(getTangent());
	float sign = temp1/abs(temp1);
	return sign * chassis->getLinearVelocity().length();
}

void Car::SetSpeed( float speed )
{
	m_Speed = speed;
}

float Car::GetForwardForceModifier()
{
	return forwardForceModifier;
}

btCollisionObject* Car::getPhysicsObject(){
	return physicsObject;
}

std::string Car::toString()
{

	std::string st = Entity::toString();

	std::stringstream stream;

	stream << "\nPowerup: ";

	for(int i = 0; i < MAX_POWERUPS; i++)
	{
		//int type = GetPowerUpAt(i).GetType();
		//printf("Type %d\n", type);

		//stream << GetPowerUpAt(i).GetType() << ", ";
	}

	std::string output = st/* + stream.str() */;

	return output;
}

void Car::outputPowerups()
{
	printf("Powerups: ");
	for(int i = 0; i < MAX_POWERUPS; i++)
	{
		int type = GetPowerUpAt(i)->GetType();
		//printf("Type %d\n", type);
		printf("%d, ",type); 
		//ss << GetPowerUpAt(i).GetType() << ", ";
	}
	printf("\n");
}

//Right now possible issue with cars above each other.
Car* Car::getClosestCar(bool inFront)
{
	btScalar distance(detectionRange);
	int index = -1;
	btAlignedObjectArray<Car*>* cList = EntityManager::getInstance()->getCarList();
	for(int i = 0; i < cList->size(); i++)
	{
		if (i == id)
			continue;
		Car* c = cList->at(i);
		btVector3 directionVect = getPosition() - c->getPosition();
		btScalar tempDist(directionVect.length());
		if (inFront)
		{
			if (directionVect.dot(getTangent()) < 0.03)
				continue;
		}
		if (tempDist < distance)
		{
			distance = tempDist;
			index = i;
		}
	}
	if (index != -1)
		return cList->at(index);
	return NULL;
}

void Car::RotatePowerups( bool RotateLeft )
{
	if( RotateLeft )
	{
		int temp = m_CarPowerUps[0].GetType();
		m_CarPowerUps[0].SetType( m_CarPowerUps[1].GetType() );
		m_CarPowerUps[1].SetType( m_CarPowerUps[2].GetType() );
		m_CarPowerUps[2].SetType( temp );
	}
	else
	{
		int temp = m_CarPowerUps[0].GetType();
		m_CarPowerUps[0].SetType( m_CarPowerUps[2].GetType() );
		m_CarPowerUps[2].SetType( m_CarPowerUps[1].GetType() );
		m_CarPowerUps[1].SetType( temp );
	}
}

void Car::setBeingSlowed(){
	beingSlowed = true;
}