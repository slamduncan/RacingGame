#include "EntityManager.h"

EntityManager* EntityManager::instance = 0;


EntityManager::EntityManager()
{
	// HACK ISH need to fix
	track = NULL;
}

/*
*	Clean up
*/
EntityManager::~EntityManager()
{
	for(int i = 0; i < carList.size(); i++)
	{
		delete carList[i];
	}
	carList.clear();

/*
	for(int i = 0; i < powerUpList.size(); i++)
	{
		delete powerUpList[i];
	}
	powerUpList.clear();
*/
/*
	for(int i = 0; i < obstacleList.size(); i++)
	{
		delete obstacleList[i];
	}
	obstacleList.clear();
*/
	if(track != NULL)
	{
		delete track;
	}
}

void EntityManager::addCar(Car* car)
{
	carList.push_back(car);
}
void EntityManager::addTrack(Track* track)
{
	this->track = track;
}
void EntityManager::addPowerUp()
{

}
void EntityManager::addObstacle()
{

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


int EntityManager::numCars()
{
	return carList.size();
}
int EntityManager::numPowerUps()
{
	return 0;
}
int EntityManager::numObstacles()
{
	return 0;
}

void EntityManager::resetCarPosition(int index, btVector3 &position)
{
	btTransform transform = carList[index]->physicsObject->getWorldTransform();

	transform.setOrigin(position);

	carList[index]->physicsObject->setWorldTransform(transform);

}

void EntityManager::resetCarOrientation(int index)
{
	btTransform transform = carList[index]->physicsObject->getWorldTransform();

	btVector3 position = transform.getOrigin();

	transform.setIdentity();

	transform.setOrigin(position);

	carList[index]->physicsObject->setWorldTransform(transform);
}







btAlignedObjectArray<Car*>* EntityManager::getCarList()
{
	return &carList;
}

Track* EntityManager::getTrack()
{
	return track;
}