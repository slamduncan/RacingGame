#include "Physics.h"
#include "EntityManager.h"
#include "Renderer.h"

//HIIIIII
Physics* Physics::physInstance = 0;
EntityManager* entityManager;
Renderer* physRender;

Physics* Physics::Inst(void){	
	if(physInstance == 0){
		physInstance = new Physics();
	}
	return physInstance;
}

Physics::Physics(void)
{
	/*
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	*/
	entityManager = EntityManager::getInstance();
	physRender = Renderer::getInstance();

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
}

Physics::~Physics(void)
{

	// Need to clean up the dynamic world since there are still entities in it
	dynamicsWorld->getCollisionObjectArray().clear();
	
	delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void Physics::step()
{	
	float MAGICSPRINGCONSTANT = 2;
	float MAGICEQUILIBRIUMLENGTH = 10;
	//void drawLine(btVector3 &start, btVector3 &end, int r, int g, int b, float width = 1.0);
	//Springs
	for(int i=0; i<entityManager->getCarList()->size(); i++){
		//Collect a car from the entity manager
		Car * c = entityManager->getCarList()->at(i);
		
		//Shoot a ray for each wheel.
		//HACK: MANUALLY USING CAR DIMENSIONS (2.5,2.5,5.0)
		btVector3 wheel1 = c->getPosition() + btVector3(-1.25,-1.25,-4.0);
		btVector3 wheel2 = c->getPosition() + btVector3(+1.25,-1.25,-4.0);
		btVector3 wheel3 = c->getPosition() + btVector3(-1.25,-1.25,+4.0);
		btVector3 wheel4 = c->getPosition() + btVector3(+1.25,-1.25,+4.0);

		btVector3 shot1 = wheel1 - c->getNormal()*MAGICEQUILIBRIUMLENGTH;
		btVector3 shot2 = wheel2 - c->getNormal()*MAGICEQUILIBRIUMLENGTH;
		btVector3 shot3 = wheel3 - c->getNormal()*MAGICEQUILIBRIUMLENGTH;
		btVector3 shot4 = wheel4 - c->getNormal()*MAGICEQUILIBRIUMLENGTH;

		btCollisionWorld::ClosestRayResultCallback RayCallback1(wheel1,shot1);
		btCollisionWorld::ClosestRayResultCallback RayCallback2(wheel2,shot2);
		btCollisionWorld::ClosestRayResultCallback RayCallback3(wheel3,shot3);
		btCollisionWorld::ClosestRayResultCallback RayCallback4(wheel4,shot4);

		dynamicsWorld->rayTest(wheel1, shot1, RayCallback1);
		dynamicsWorld->rayTest(wheel2, shot2, RayCallback2);
		dynamicsWorld->rayTest(wheel3, shot3, RayCallback3);
		dynamicsWorld->rayTest(wheel4, shot4, RayCallback4);

		if(RayCallback1.hasHit()) {
			btVector3 hit = RayCallback1.m_hitPointWorld;
			btVector3 hitNormal = RayCallback1.m_hitNormalWorld;

			//physRender->drawLine(wheel1,hit,255,0,0,1.0);

			//Equilibrium length settable above
			if((hit-wheel1).length()<MAGICEQUILIBRIUMLENGTH){
				//force = -MAGICSPRINGCONSTANT*displacement vector
				btVector3 force = c->getNormal() * (10.0f - (float)((hit-wheel1).length())) * MAGICSPRINGCONSTANT;
				//printf("%f\n",(hit-wheel1).length());

				c->physicsObject->applyForce(force,wheel1);
			}
		}

		if(RayCallback2.hasHit()) {
			btVector3 hit = RayCallback2.m_hitPointWorld;
			btVector3 hitNormal = RayCallback2.m_hitNormalWorld;

			//physRender->drawLine(wheel2,hit,255,0,0,1.0);

			//Equilibrium length settable above
			if((hit-wheel2).length()<MAGICEQUILIBRIUMLENGTH){
				//force = -MAGICSPRINGCONSTANT*displacement vector
				btVector3 force = c->getNormal() * (10.0f - (float)((hit-wheel2).length())) * MAGICSPRINGCONSTANT;
				//printf("%f\n",(hit-wheel1).length());

				c->physicsObject->applyForce(force,wheel2);
			}
		}

		if(RayCallback3.hasHit()) {
			btVector3 hit = RayCallback3.m_hitPointWorld;
			btVector3 hitNormal = RayCallback3.m_hitNormalWorld;

			//physRender->drawLine(wheel3,hit,255,0,0,1.0);

			//Equilibrium length settable above
			if((hit-wheel3).length()<MAGICEQUILIBRIUMLENGTH){
				//force = -MAGICSPRINGCONSTANT*displacement vector
				btVector3 force = c->getNormal() * (10.0f - (float)((hit-wheel3).length())) * MAGICSPRINGCONSTANT;
				//printf("%f\n",(hit-wheel1).length());

				c->physicsObject->applyForce(force,wheel3);
			}
		}

		if(RayCallback1.hasHit()) {
			btVector3 hit = RayCallback4.m_hitPointWorld;
			btVector3 hitNormal = RayCallback4.m_hitNormalWorld;

			//physRender->drawLine(wheel4,hit,255,0,0,1.0);

			//Equilibrium length settable above
			if((hit-wheel4).length()<MAGICEQUILIBRIUMLENGTH){
				//force = -MAGICSPRINGCONSTANT*displacement vector
				btVector3 force = c->getNormal() * (10.0f - (float)((hit-wheel4).length())) * MAGICSPRINGCONSTANT;
				//printf("%f\n",(hit-wheel1).length());

				c->physicsObject->applyForce(force,wheel4);
			}
			
		}
	}
	//physRender->updateGL();
 
	dynamicsWorld->stepSimulation(1/60.f,10);



	//btCollisionObjectArray temp = dynamicsWorld->getCollisionObjectArray();

	//btCollisionObject* temp2 = temp.at(0);

	//btTransform trans = temp2->getWorldTransform();

	

	//printf("car data: %f,%f,%f\n", trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()); 
}

void Physics::setGravity(const btVector3 &gravity)
{
	dynamicsWorld->setGravity(gravity);
}

void Physics::addEntity(const Entity &ent)
{
	dynamicsWorld->addRigidBody(ent.physicsObject);
}

void Physics::removeEntity(const Entity &ent)
{
	dynamicsWorld->removeRigidBody(ent.physicsObject);
}

void Physics::setDebugDrawer(btIDebugDraw *debugDrawer)
{
	dynamicsWorld->setDebugDrawer(debugDrawer);	
}

void Physics::debugDraw()
{
	dynamicsWorld->debugDrawWorld();
}

void Physics::setDebugLevel(int level)
{
	dynamicsWorld->getDebugDrawer()->setDebugMode(level);
}