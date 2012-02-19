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
//#include "PowerUp.h"
#include "LinearMath\btAlignedObjectArray.h"

class EntityManager
{
private:
	btAlignedObjectArray<Car*> carList;
	//btAlignedObjectArray<PowerUp*> powerUpList;
	//btAlignedObjectArray<Obstacle*> obstacleList;
	btAlignedObjectArray<Waypoint*> waypointList;

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
		WAYPOINT
	};

	Car* getCar(int index);
	Waypoint* getWaypoint(int index);


	void createCar(char* path, btScalar &mass, btTransform &trans);
	void createTrack(char* path, btTransform &trans);
	void createWaypoint(char* path, btTransform &trans);
	void createObstacle(char* path, btScalar &mass, btTransform &trans);

	void addCar(Car* car);
	void addTrack(Track* track);
	void addPowerUp();
	void addObstacle();
	void addWaypoint(Waypoint* waypoint);
	
	void removeCar();
	void removeTrack();
	void removePowerUp();
	void removeObstacle();
	void removeWaypoint();

	int numCars();
	int numPowerUps();
	int numObstacles();
	int numWaypoints();

	void resetCarPosition(int index, btVector3 &position);
	void resetCarOrientation(int index);
	void resetCar(int index, btVector3 &position);

	btAlignedObjectArray<Car*>* getCarList();
	btAlignedObjectArray<Waypoint*>* getWaypointList();
	Track* getTrack();
	// btAlignedObjectArray<PowerUp*>* getPowerUpList();
	// btAlignedObjectArray<Obstacle*>* getObstacleList();

};

#endif
