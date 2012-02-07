#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
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

	btAlignedObjectArray<Car*>* getCarList();
	btAlignedObjectArray<Waypoint*>* getWaypointList();
	Track* getTrack();
	// btAlignedObjectArray<PowerUp*>* getPowerUpList();
	// btAlignedObjectArray<Obstacle*>* getObstacleList();

};

#endif
