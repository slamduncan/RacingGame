#include "EntityManager.h"

EntityManager* EntityManager::instance = 0;


EntityManager::EntityManager()
{
	// need to find a better way to store the track
	track = NULL;
}

/*
*	Clean up
*/
EntityManager::~EntityManager()
{
	for(int i = 0; i < carList.size(); i++)
	{
		if(carList[i])
			delete carList[i];
	}
	carList.clear();

	for(int i = 0; i < waypointList.size(); i++)
	{
		if(waypointList[i])
			delete waypointList[i];
	}
	waypointList.clear();


	for(int i = 0; i < powerUpList.size(); i++)
	{
		if(powerUpList[i])
			delete powerUpList[i];
	}
	powerUpList.clear();

/*
	for(int i = 0; i < obstacleList.size(); i++)
	{
		delete obstacleList[i];
	}
	obstacleList.clear();
*/

	for(int i = 0; i < spawnList.size(); i++)
	{
		if(spawnList[i])
			delete spawnList[i];
	}
	spawnList.clear();

	if(track != NULL)
	{
		delete track;
	}
}

Car* EntityManager::getCar(int index)
{
	assert(index >= 0 && index < numCars());

	return carList[index];
}

Waypoint* EntityManager::getWaypoint(int index)
{
	assert(index >= 0 && index < numWaypoints());

	return waypointList[index];
}

PowerUp* EntityManager::getPowerup(int index)
{
	assert(index >=0 && index < numPowerUps());

	return powerUpList[index];
}

Spawnable* EntityManager::getSpawnable(int index)
{
	assert(index >=0 && index < numSpawnable());

	return spawnList[index];
}

SlowField* EntityManager::getSlowField(int index)
{
	return slowFieldList[index];
}

Mine* EntityManager::getMine(int index)
{
	return mineList[index];
}

void EntityManager::createCar(char* path, btScalar &mass, btTransform &trans)
{
	btScalar width = btScalar(10.0f);
	btScalar height = btScalar(5.f);
	btScalar depth = btScalar(5.f);

	Car* car = new Car();
	car->carMass = mass;
	car->id = carList.size();

	car->initRenderObject(path);
	
	btCollisionShape* boxShape = sFactory.createBox(width, height, depth);

	car->initPhysicsObject(boxShape, mass, trans);

	if (carList.size() < 1)
		car->initObservers();

	addCar(car);

	Physics::Inst()->addRigidBody(*car);

	//phyEngine->addEntity(*car);
}

void EntityManager::createTrack(char* path, btTransform &trans)
{
	btScalar mass = btScalar(0.f);

	Track* trk = new Track();

	trk->initRenderObject(path);

	btCollisionShape* triMesh = sFactory.createStaticTriangleMesh(trk->renderObject);

	trk->initPhysicsObject(triMesh, mass, trans);

	addTrack(trk);

	Physics::Inst()->addEntity(*trk);
}

void EntityManager::createWaypoint(char* path, btTransform &trans, int carThrottle)
{
	btScalar mass = btScalar(0.f);
	
	Waypoint* wp = new Waypoint();

	wp->initRenderObject(path);

	btCollisionShape* triMesh = sFactory.createStaticTriangleMesh(wp->renderObject);

	wp->initPhysicsObject(triMesh, mass, trans);

	wp->initObservers();

	wp->setThrottle(carThrottle);

	addWaypoint(wp);
}

void EntityManager::createPowerup(char* path, btTransform &trans)
{
	btScalar mass = btScalar(0.f);
	
	PowerUp* pup = new PowerUp();
	pup->SetType(3);

	pup->initRenderObject(path);
	
	//Taking this out for a bit; doesn't need to be a trimesh really
	//btCollisionShape* triMesh = sFactory.createStaticTriangleMesh(pup->renderObject);
	btScalar radius = 2.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);

	pup->initPhysicsObject(sphereMesh, mass, trans);

	addPowerUp(pup);

	Physics::Inst()->addEntity(*pup);
}

void EntityManager::createObstacle(char* path, btScalar &mass, btTransform &trans)
{

}

void EntityManager::createSpawnable(char* path, btTransform &trans)
{
	btScalar mass = btScalar(0.f);
	
	Spawnable* sp = new Spawnable();

	sp->initRenderObject(path);

	btScalar radius = 2.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);

	sp->initPhysicsObject(sphereMesh, mass, trans);

	addSpawnable(sp);

	Physics::Inst()->addEntity(*sp);
}

void EntityManager::createRocket(int startingWaypoint, btTransform &trans, int carId)
{
	btScalar mass = btScalar(0.f);
	
	Spawnable* sp = new Rocket(startingWaypoint, carId);

	sp->initRenderObject("model/Rocket.dae");

	btScalar radius = 2.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);

	// I think our problem is here!
	sp->initPhysicsObject(sphereMesh, mass, trans);

	addSpawnable(sp);

	Physics::Inst()->addEntity(*sp);
}

void EntityManager::createShield(Car *c)
{
	btScalar mass = btScalar(0.f);
	
	Spawnable* sp = new Shield(c);
	sp->carId = c->id;

	bool temp = sp->initRenderObject("model/shield.dae");

	btScalar radius = 2.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);

	
	sp->initPhysicsObject(sphereMesh, mass, c->getPhysicsObject()->getWorldTransform());

	addSpawnable(sp);

	Physics::Inst()->addEntity(*sp);
}


void EntityManager::createSlowField(Car* c)
{
	SlowField* sf = new SlowField(c);
	
	sf->initRenderObject("model/powerup.lwo");
	sf->carId = c->id;
	
	btCompoundShape* blobContainer = new btCompoundShape();

	btScalar mass = btScalar(0.f);

	btTransform carTr = c->physicsObject->getWorldTransform();
	btVector3 carOr = carTr.getOrigin();
	carOr = carOr + c->getTangent() * 15.0f;
	carTr.setOrigin(carOr);

	sf->initPhysicsObject(blobContainer, mass, carTr);

	btScalar radius = btScalar(7.5f);

	btCollisionShape* sphere = sFactory.createSphere(radius);

	btTransform cT = c->physicsObject->getWorldTransform();

	cT.setOrigin(btVector3(0, 0, 0));
	cT.setRotation(c->physicsObject->getWorldTransform().getRotation());

	sf->blobContainer->getAngularMotionDisc();

	sf->blobContainer->addChildShape(cT, sphere); 

	addSlowField(sf);

	Physics::Inst()->addGhost(btGhostObject::upcast(sf->physicsObject));
}

void EntityManager::createSlowFieldSpawnable(char* path, SlowField* sf)
{
	//MAGIC NUMBER! UH OHHH! :D!
	btScalar radius = 7.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);
	
	btVector3 containerPos = sf->physicsObject->getWorldTransform().getOrigin();
	
	btTransform carT = sf->getCar()->chassis->getWorldTransform();
	btVector3 carPos = carT.getOrigin();

	btVector3 diff = carPos - containerPos + sf->getCar()->getTangent() * 15.0f;
	//printf("DIFF: (%f, %f, %f)\n", diff.x(), diff.y(), diff.z());

	btMatrix3x3 parentOrientation = sf->physicsObject->getWorldTransform().getBasis();
	parentOrientation = parentOrientation.inverse();
	carT.setOrigin(parentOrientation*diff);


	sf->blobContainer->addChildShape(carT, sphereMesh);

}

void EntityManager::createMine(Car* c, char* path)
{
	Mine* mine = new Mine(c);

	mine->initRenderObject(path);
	mine->carId = c->id;
	
	btScalar radius = 2.5f;
	btCollisionShape* sphereMesh = sFactory.createSphere(radius);

	btTransform carT = c->chassis->getWorldTransform();
	btVector3 carPos = carT.getOrigin();
	carPos = carPos + c->getTangent() * 15.0f;
	carT.setOrigin(carPos);

	mine->initPhysicsObject(sphereMesh, carT);
	addMine(mine);

	Physics::Inst()->addEntity(*mine);
}


void EntityManager::addCar(Car* car)
{
	carList.push_back(car);
}

// THIS IS VERY UNSAFE ATM
// We need a way to remove the track from the physics world first before we delete it.
void EntityManager::addTrack(Track* track)
{
	this->track = track;	// will memleak if we try to create another track
}
void EntityManager::addPowerUp(PowerUp* powerup)
{
	powerUpList.push_back(powerup);
}
void EntityManager::addObstacle()
{

}
void EntityManager::addWaypoint(Waypoint* waypoint)
{
	waypoint->setIndex(waypointList.size());
	waypointList.push_back(waypoint);	
}

void EntityManager::addSpawnable(Spawnable* spawn)
{
	spawnList.push_back(spawn);
}

void EntityManager::addSlowField(SlowField* slow){
	slowFieldList.push_back(slow);
}

void EntityManager::addMine(Mine* mine){
	mineList.push_back(mine);
}


void EntityManager::removeCar()
{

}
void EntityManager::removeTrack()
{

}
void EntityManager::removePowerUp()
{

}
void EntityManager::removeObstacle()
{

}
void EntityManager::removeWaypoint()
{
	
}

void EntityManager::removeSpawnable(Spawnable * spawnable)
{
	Physics::Inst()->removeEntity(*spawnable);
	spawnList.remove(spawnable);
	spawnable->~Spawnable();
}

void EntityManager::removeSlowField(SlowField * sf)
{
	Physics::Inst()->removeEntity(*sf);
	slowFieldList.remove(sf);
	//Just in case
	sf->~SlowField();
}

void EntityManager::removeMine(Mine* mine)
{
	Physics::Inst()->removeEntity(*mine);
	mineList.remove(mine);
	mine->~Mine();
}

int EntityManager::numCars()
{
	return carList.size();
}
int EntityManager::numPowerUps()
{
	return powerUpList.size();
}
int EntityManager::numObstacles()
{
	return 0;
}
int EntityManager::numWaypoints()
{
	return waypointList.size();
}

int EntityManager::numSpawnable()
{
	return spawnList.size();
}

int EntityManager::numSlowField()
{
	return slowFieldList.size();
}

void EntityManager::resetCarPosition(int index, btVector3 &position)
{
	btTransform transform = carList[index]->physicsObject->getWorldTransform();

	transform.setOrigin(position);

	carList[index]->physicsObject->setWorldTransform(transform);

}


void EntityManager::resetCar(int index, btTransform &transform)
{
	//btTransform transform = carList[index]->physicsObject->getWorldTransform();

	//transform.setOrigin(position);
	carList[index]->chassis->clearForces();
	carList[index]->chassis->setLinearVelocity(btVector3(0, 0, 0));
	carList[index]->chassis->setAngularVelocity(btVector3(0, 0, 0));
	carList[index]->physicsObject->setWorldTransform(transform);

}

void EntityManager::resetCarOrientation(int index)
{
	btTransform transform = carList[index]->physicsObject->getWorldTransform();

	//btVector3 position = transform.getOrigin();

	transform.setBasis(btMatrix3x3(btQuaternion(0, 1, 0, 1)));

	carList[index]->physicsObject->setWorldTransform(transform);
}

void EntityManager::resetCar(int index, btVector3 &position)
{
	resetCarPosition(index, position);
	resetCarOrientation(index);

	carList[index]->chassis->clearForces();

	carList[index]->chassis->setLinearVelocity(btVector3(0, 0, 0));
	carList[index]->chassis->setAngularVelocity(btVector3(0, 0, 0));
}

btAlignedObjectArray<Car*>* EntityManager::getCarList()
{
	return &carList;
}
btAlignedObjectArray<Waypoint*>* EntityManager::getWaypointList()
{
	return &waypointList;
}

Track* EntityManager::getTrack()
{
	return track;
}

btAlignedObjectArray<PowerUp*>* EntityManager::getPowerUpList()
{
	return &powerUpList;
}

btAlignedObjectArray<Spawnable*>* EntityManager::getSpawnableList()
{
	return &spawnList;
}

btAlignedObjectArray<SlowField*>* EntityManager::getSlowFieldList()
{
	return &slowFieldList;
}

btAlignedObjectArray<Mine*>* EntityManager::getMineList(){
	return &mineList;
}

int EntityManager::getCarIndexViaPointer(btCollisionObject* p){
	for(int i=0; i < carList.size(); i++){
		if(p == carList.at(i)->physicsObject){
			return i;
		}
	}
	return -1;
}