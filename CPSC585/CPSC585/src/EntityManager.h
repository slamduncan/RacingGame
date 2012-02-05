#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "Entity.h"
#include "Car.h"
#include "Track.h"
//#include "PowerUp.h"
#include "LinearMath\btAlignedObjectArray.h"

class EntityManager
{
private:
	btAlignedObjectArray<Car*> carList;
	//btAlignedObjectArray<PowerUp*> powerUpList;
	//btAlignedObjectArray<Obstacle*> obstacleList;

	Track* track;

	static EntityManager* instance;

	EntityManager();

public:

	~EntityManager();

	static EntityManager* getInstance()
	{
		if(instance == NULL)
		{
			instance = new EntityManager();
		}

		return instance;
	}

	enum EntityType
	{
		CAR = 0,
		TRACK,
		POWERUP,
		OBSTACLE
	};

	void addCar(Car* car);
	void addTrack(Track* track);
	void addPowerUp();
	void addObstacle();
	
	void removeCar();
	void removeTrack();
	void removePowerUp();
	void removeObstacle();

	int numCars();
	int numPowerUps();
	int numObstacles();

	btAlignedObjectArray<Car*>* getCarList();
	Track* getTrack();

};

#endif
