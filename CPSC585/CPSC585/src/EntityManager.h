#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

//#define NDEBUG
#include <assert.h>


#include "ShapeFactory.h"
#include "Physics.h"
#include "Entity.h"
#include "Car.h"
#include "Track.h"
#include "Waypoint.h"
#include "PowerUp.h"
#include "Spawnable.h"
#include "LinearMath\btAlignedObjectArray.h"

class EntityManager
{
private:
	btAlignedObjectArray<Car*> carList;
	btAlignedObjectArray<PowerUp*> powerUpList;
	//btAlignedObjectArray<Obstacle*> obstacleList;
	btAlignedObjectArray<Waypoint*> waypointList;
	btAlignedObjectArray<Spawnable*> spawnList;

	Track* track;

	static EntityManager* instance;

	EntityManager();

	ShapeFactory sFactory;

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
		OBSTACLE,
		WAYPOINT,
		SPAWNABLE
	};

	Car* getCar(int index);
	Waypoint* getWaypoint(int index);
	PowerUp* getPowerup(int index);


	void createCar(char* path, btScalar &mass, btTransform &trans);
	void createTrack(char* path, btTransform &trans);
	void createWaypoint(char* path, btTransform &trans, int carThrottle = -32767);
	void createPowerup(char* path, btTransform &trans);
	void createObstacle(char* path, btScalar &mass, btTransform &trans);
	void createSpawnable(char* path, btTransform &trans);

	void addCar(Car* car);
	void addTrack(Track* track);
	void addPowerUp(PowerUp* powerup);
	void addObstacle();
	void addWaypoint(Waypoint* waypoint);
	void addSpawnable(Spawnable* spawn);
	
	void removeCar();
	void removeTrack();
	void removePowerUp();
	void removeObstacle();
	void removeWaypoint();
	void removeSpawnable();

	int numCars();
	int numPowerUps();
	int numObstacles();
	int numWaypoints();
	int numSpawnable();

	void resetCarPosition(int index, btVector3 &position);
	void resetCar(int index, btTransform &transform);
	void resetCarOrientation(int index);
	void resetCar(int index, btVector3 &position);

	btAlignedObjectArray<Car*>* getCarList();
	btAlignedObjectArray<Waypoint*>* getWaypointList();
	Track* getTrack();
	btAlignedObjectArray<PowerUp*>* getPowerUpList();
	// btAlignedObjectArray<Obstacle*>* getObstacleList();
	btAlignedObjectArray<Spawnable*>* getSpawnableList();


	int getCarIndexViaPointer(btCollisionObject* p);
};

#endif
