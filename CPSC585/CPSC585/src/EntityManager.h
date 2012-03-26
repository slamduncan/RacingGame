#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

//#define NDEBUG
#include <assert.h>

#include "ModelManager.h"
#include "ShapeFactory.h"
#include "Physics.h"
#include "Entity.h"
#include "Car.h"
#include "Track.h"
#include "Waypoint.h"
#include "PowerUp.h"
#include "Spawnable.h"
#include "Rocket.h"
#include "LinearMath\btAlignedObjectArray.h"
#include "Slowfield.h"
#include "Shield.h"
#include "Mine.h"

class EntityManager
{
private:
	btAlignedObjectArray<Car*> carList;
	btAlignedObjectArray<PowerUp*> powerUpList;
	//btAlignedObjectArray<Obstacle*> obstacleList;
	btAlignedObjectArray<Waypoint*> waypointList;
	btAlignedObjectArray<Spawnable*> spawnList;
	btAlignedObjectArray<SlowField*> slowFieldList;
	btAlignedObjectArray<Mine*> mineList;

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
	Spawnable* getSpawnable(int index);
	SlowField* getSlowField(int index);
	Mine* getMine(int index);

	void createCar(char* path, btScalar &mass, btTransform &trans);
	void createTrack(char* path, btTransform &trans);
	void createWaypoint(char* path, btTransform &trans, int carThrottle = -32767);
	void createPowerup(char* path, btTransform &trans);
	void createObstacle(char* path, btScalar &mass, btTransform &trans);
	void createSpawnable(char* path, btTransform &trans);
	void createRocket(int startingWaypoint, btTransform &trans, int carId);
	void createSlowField(Car* c);
	void createSlowFieldSpawnable(char* path, SlowField* sf);
	void createShield(Car* c);
	void createMine(Car* c, char* path);

	void addCar(Car* car);
	void addTrack(Track* track);
	void addPowerUp(PowerUp* powerup);
	void addObstacle();
	void addWaypoint(Waypoint* waypoint);
	void addSpawnable(Spawnable* spawn);
	void addSlowField(SlowField* slow);
	void addMine(Mine* mine);
	
	void removeCar();
	void removeTrack();
	void removePowerUp();
	void removeObstacle();
	void removeWaypoint();
	void removeSpawnable(Spawnable * spawnable);
	void removeSlowField(SlowField * sf);
	void removeMine(Mine* mine);

	int numCars();
	int numPowerUps();
	int numObstacles();
	int numWaypoints();
	int numSpawnable();
	int numSlowField();
	int numMines();

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
	btAlignedObjectArray<SlowField*>* getSlowFieldList();
	btAlignedObjectArray<Mine*>* getMineList();

	int getCarIndexViaPointer(btCollisionObject* p);
};

#endif
