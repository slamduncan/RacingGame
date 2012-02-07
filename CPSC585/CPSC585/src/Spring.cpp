
#include "Spring.h"

Spring::Spring(btRigidBody* physics){
	physicsObject = physics;
	MAGICEQUILIBRIUMLENGTH = 4;
	MAGICSPRINGCONSTANT = 20;
	dynamicsWorld = Physics::Inst()->getDiscreteDynamicsWorld();

}

Spring::Spring(void)
{	

}

Spring::~Spring(void)
{
}

void Spring::update(btVector3 &springLocation, btVector3 &carNormal)	
{
	btVector3 equalibriumPos = springLocation - carNormal*MAGICEQUILIBRIUMLENGTH;
	btCollisionWorld::ClosestRayResultCallback RayCallback(springLocation,equalibriumPos);

	dynamicsWorld->rayTest(springLocation, equalibriumPos, RayCallback);

	if(RayCallback.hasHit()) {
		btVector3 hit = RayCallback.m_hitPointWorld;
		btVector3 hitNormal = RayCallback.m_hitNormalWorld;

		//physRender->drawLine(wheel1,hit,255,0,0,1.0);

		//Equilibrium length settable above
		if((hit - springLocation).length()<MAGICEQUILIBRIUMLENGTH){
			
			btVector3 force = btVector3(0,1,0)* (MAGICEQUILIBRIUMLENGTH - ((hit-springLocation).length())) * MAGICSPRINGCONSTANT;			
			physicsObject->applyForce(force,springLocation);
		}
	}
}