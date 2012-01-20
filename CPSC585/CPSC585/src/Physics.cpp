#include "Physics.h"
#include "btBulletCollisionCommon.h"

//HIIIIII
Physics* Physics::physInstance = 0;

Physics* Physics::Inst(void){	
	if(physInstance == 0){
		physInstance = new Physics();
	}
	return physInstance;
}

Physics::Physics(void)
{

}

Physics::~Physics(void)
{
	
}
