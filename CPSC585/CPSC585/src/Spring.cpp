
#include "Spring.h"

Spring::Spring(btRigidBody* physics){
	physicsObject = physics;
	MAGICEQUILIBRIUMLENGTH = 3;
	MAGICSPRINGCONSTANT = 10;
	dynamicsWorld = Physics::Inst()->getDiscreteDynamicsWorld();

	springLength = MAGICEQUILIBRIUMLENGTH;
	force = btVector3(0, 0, 0);

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

	btScalar MAGICDAMPERCONSTANT = 10;

	springLength = MAGICEQUILIBRIUMLENGTH;

	if(RayCallback.hasHit()) {
		btVector3 hit = RayCallback.m_hitPointWorld;
		btVector3 hitNormal = RayCallback.m_hitNormalWorld;

		//physRender->drawLine(wheel1,hit,255,0,0,1.0);

		springLength = (hit - springLocation).length();

		//Equilibrium length settable above
		if((hit - springLocation).length()<MAGICEQUILIBRIUMLENGTH){
			//btVector3 force = hitNormal*MAGICSPRINGCONSTANT*(MAGICEQUILIBRIUMLENGTH-3) - hitNormal.dot(carAngularVelocity)*MAGICDAMPERCONSTANT*hitNormal;

			btScalar forceCoefficient = (MAGICEQUILIBRIUMLENGTH - ((hit-springLocation).length())) * MAGICSPRINGCONSTANT;
			
			//forceCoefficient -= 3;
			/*
			if(forceCoefficient < 0)
			{
				forceCoefficient = 0;
			}
			*/
			
			btVector3 linV = physicsObject->getLinearVelocity();


			force = carNormal*forceCoefficient - 2*linV;
			//printf("(%f %f %f)", force.getX(),force.getY(),force.getZ());
			//force = force - physicsObject->getAngularVelocity();
			//printf(" (%f %f %f)\n", force.getX(),force.getY(),force.getZ());

			//physicsObject->applyForce(force,springLocation);

			physicsObject->applyForce(force, springLocation);

		}

	}
}