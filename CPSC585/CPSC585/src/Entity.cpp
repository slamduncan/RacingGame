#include "Entity.h"
#include "EventSystemHandler.h"

Entity::Entity(): rotationObserver(this, &Entity::observeRotation)
{
	renderObject = NULL;
	physicsObject = NULL;
	isInit = init();
	
	//loadObj("../CPSC585/model/box.obj");
}

Entity::Entity(char* filename, btScalar &mass, btTransform &trans) : rotationObserver(this, &Entity::observeRotation)
{
	if(filename != NULL)
	{
		renderObject = NULL;
		physicsObject = NULL;

		isInit = init();
		loadObj(filename, mass, trans);
	}
}

/*
*	Clean up
*/
Entity::~Entity()
{
	// if there is a render object, clean up
	if(renderObject != NULL)
	{
		delete renderObject;
	}
	// if there is a physics object, clean up
	if(physicsObject != NULL)
	{
		delete physicsObject;
	}
}

/*
*	initializes the entity at (0, 0, 0)
*	with a standard frenet frame of:
*	tangent = (0, 0, 1);
*	normal = (0, 1, 0);
*	binormal = (1, 0, 0);
*
*	Return:
*	indicates that the entity has been initialized
*/
bool Entity::init()
{

	position = btVector3(0,0,0);
	
	tangent = btVector3(0, 0, 1);
	normal = btVector3(0, 1, 0);
	binormal = normal.cross(tangent);

	loaded = 0;
	//renderObject = new objLoader();
	return true;
}

void Entity::initObservers(){
	//EventSystemHandler::getInstance()->addObserver(&rotationObserver, EventTypes::ROTATION);
	rotationObserver.init(EventTypes::ROTATION);

}

//
//	Need to fix this, as it can be misinterrupted as moving the entity to
//  a specific location in world space
//
void Entity::move(float x, float y, float z)
{
	btVector3 temp(x, y, z);

	position += temp;
}
void Entity::move(const btVector3 &newPos)
{
	position += newPos;
}

/*
*	Rotates the frenet frame of the object based on a axis of rotation and angle in degs.
*
*	Parameters:
*	axis - axis of rotation
*	deg - angle of rotation in degrees
*/
void Entity::rotate(const btVector3 &axis, int deg)
{
	float radian = SIMD_RADS_PER_DEG * deg;

	// quaternion q
	btQuaternion q = btQuaternion(axis, radian);

	// quaternion p
	btQuaternion tan = btQuaternion(tangent.x(), tangent.y(), tangent.z(), tangent.w());
	btQuaternion nor = btQuaternion(normal.x(), normal.y(), normal.z(), normal.w());
	btQuaternion bin = btQuaternion(binormal.x(), binormal.y(), binormal.z(), binormal.w());

	// r = qpq_c
	tan = q*tan*q.inverse();
	nor = q*nor*q.inverse();
	bin = q*bin*q.inverse();

	tangent.setX(tan.getX());
	tangent.setY(tan.getY());
	tangent.setZ(tan.getZ());

	normal.setX(nor.getX());
	normal.setY(nor.getY());
	normal.setZ(nor.getZ());

	binormal.setX(bin.getX());
	binormal.setY(bin.getY());
	binormal.setZ(bin.getZ());
	//printf("rtan (%f, %f, %f)\n", rtan.getX(), rtan.getY(), rtan.getZ());

	//tangent = btVector3(rtan);

}

btScalar* Entity::getGLMatrix()
{
	btScalar* glMat = new btScalar[16];
	btTransform trans;// = new btTransform();
	physicsObject->getMotionState()->getWorldTransform(trans);

	trans.getOpenGLMatrix(glMat);

	return glMat;
}

btVector3 Entity::getPosition()
{
	btTransform transM = physicsObject->getWorldTransform();

	btVector3 pos = transM.getOrigin();

	return pos;
}

btVector3 Entity::getTangent()
{
	btTransform transM = physicsObject->getWorldTransform();

	btMatrix3x3 orientation = transM.getBasis();

	btVector3 tan = orientation.getColumn(0);

	return tan;

}
btVector3 Entity::getNormal()
{
	btTransform transM = physicsObject->getWorldTransform();

	btMatrix3x3 orientation = transM.getBasis();

	btVector3 nor = orientation.getColumn(1);

	return nor;
}
btVector3 Entity::getBinormal()
{

	btTransform transM = physicsObject->getWorldTransform();

	btMatrix3x3 orientation = transM.getBasis();

	btVector3 bin = orientation.getColumn(2);

	return bin;
}









/*
*	Loads a given obj model to this entity.
*	Generate the render and physics representation
*
*	Parameters:
*	filename - location of the model
*
*	Return:
*	whether or not the render and physics rep loaded sucessfully
*/
bool Entity::loadObj(char* filename, btScalar &mass, btTransform &trans)
{
	//printf("loading...\n");
	
	if(isInit)
	{
		if(loaded)
		{
			delete renderObject;
		}
		if(physicsObject != NULL)
		{
			delete physicsObject;
		}
		
		renderObject = new objLoader();

		// returns 0 if fails to load
		loaded = renderObject->load(filename);



		// Generate the physics representation
		if(loaded)
		{
			btCollisionShape* objShape;
			btVector3 inertia(0,0,0);
				
			// generate triangle mesh for bullet
			btTriangleMesh* bttm = new btTriangleMesh();
			
			// generate the triangle mesh for bullet
			for(int i = 0; i < renderObject->faceCount; i++)
			{
				obj_face* face = renderObject->faceList[i];	// get a triangle

				obj_vector* vert0 = renderObject->vertexList[face->vertex_index[0]];	//v0
				obj_vector* vert1 = renderObject->vertexList[face->vertex_index[1]];	//v1
				obj_vector* vert2 = renderObject->vertexList[face->vertex_index[2]];	//v2

				btVector3 btv0 = btVector3((float)vert0->e[0], (float)vert0->e[1], (float)vert0->e[2]);
				btVector3 btv1 = btVector3((float)vert1->e[0], (float)vert1->e[1], (float)vert1->e[2]);
				btVector3 btv2 = btVector3((float)vert2->e[0], (float)vert2->e[1], (float)vert2->e[2]);
				
				bttm->addTriangle(btv0, btv1, btv2, false);	// addTriangle(v0, v1, v2, removeDupes)
			}


			// static rigid body
			if(mass == 0.f)
			{
				
				//printf("static body\n");
				objShape = new btBvhTriangleMeshShape(bttm, true, true);
				//objShape = new btStaticPlaneShape(btVector3(0, -10, 0), trans.getOrigin().getY());
			}
			// dynamic rigid body
			else
			{

				objShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
				//objShape = new btConvexTriangleMeshShape(bttm, true);

				objShape->calculateLocalInertia(mass, inertia);

				
			}			

			//btBvhTriangleMeshShape* objShape = new btBvhTriangleMeshShape(bttm, true, true);

			//btVector3 fallInertia(0, 0, 0);
			//objShape->calculateLocalInertia(1, fallInertia);

			// btDefaultMotionState(orientiation matrix, position)
			// need to pass orientation and position at the start
			btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

			// entRigidBodyCI(mass, motion state, collision shape, inertia);
			// need to update the mass to make it as a variable?

			//printf("inertia is (%f, %f, %f)\n", inertia.getX(), inertia.getY(), inertia.getZ());

			btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,objShape,inertia);

			physicsObject = new btRigidBody(entRigidBodyCI);
			//printf("collsion flags %d\n", physicsObject->getCollisionFlags());

			return true;
		}
	}

	return false;	// render and physics representations failed to init
}

/*
*	debug console output
*/
void Entity::debug()
{	
	printf("-----------------------------------------------\n");
	printf("Pos: (%f, %f, %f)\n", position.x(), position.y(), position.z());
	printf("\n");
	printf("Tan: (%f, %f, %f)\n", tangent.x(), tangent.y(), tangent.z());
	printf("Nor: (%f, %f, %f)\n", normal.x(), normal.y(), normal.z());
	printf("Bin: (%f, %f, %f)\n", binormal.x(), binormal.y(), binormal.z());
}

/*
*	Returns a string presentation of the entity
*
*	
*/
std::string Entity::toString()
{
	btVector3 pos = getPosition();
	btVector3 tan = getTangent();
	btVector3 nor = getNormal();
	btVector3 bin = getBinormal();


	std::stringstream ss;
	ss << "Pos: (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")\n";
	ss << "Tangent: (" << tan.x() << ", " << tan.y() << ", " << tan.z() << ")\n";
	ss << "Normal: (" << nor.x() << ", " << nor.y() << ", " << nor.z() << ")\n";
	ss << "Binormal: (" << bin.x() << ", " << bin.y() << ", " << bin.z() << ")";

	return ss.str();
}