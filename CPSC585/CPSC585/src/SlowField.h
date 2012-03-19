#pragma once
#include "Spawnable.h"
#include "Car.h"

class SlowField:Spawnable
{
public:
	SlowField(Car* car);
	~SlowField(void);

private:
	Car * car;
};
