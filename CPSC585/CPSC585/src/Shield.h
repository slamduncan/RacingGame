#ifndef SHIELD_H
#define SHIELD_H

#include "Spawnable.h"
#include "Car.h"

class Shield : public Spawnable
{
private:
	Car* carRef;

public:
	Shield(Car* c);
	~Shield();	
	void updateLocation();
};

#endif SHIELD_H