#include "Entity.h"
#include "EventSystemHandler.h"

Entity::Entity() : rotationObserver(this, &Entity::observeRotation), forwardForceObserver(this, &Entity::observeForwardForce)
{
	physicsObject = NULL;
	renderObject = NULL;

	glMatrix = new btScalar[16];
	


	//loadObj("../CPSC585/model/box.obj");
}

/*
Entity::Entity(char* filename, btScalar &mass, btTransform &trans) : rotationObserver(this, &Entity::observeRotation), forwardForceObserver(this, &Entity::observeForwardForce)
{
	if(filename != NULL)
	{
		//renderObject = NULL;
		physicsObject = NULL;

		isInit = init();
		//loadObj(filename, mass, trans);
	}
}
*/

/*
*	Clean up
*/
Entity::~Entity()
{
	if(renderObject != NULL)
	{
		aiReleaseImport(renderObject);
	}

	// if there is a physics object, clean up
	if(physicsObject != NULL)
	{
		delete physicsObject;
	}

	delete[] glMatrix;
}

/*
*	Return:
*	indicates that the entity has been initialized
*/
/*
bool Entity::init()
{

	return true;
}
*/

void Entity::initObservers()
{
	//EventSystemHandler::getInstance()->addObserver(&rotationObserver, EventTypes::ROTATION);
	rotationObserver.init(EventTypes::ROTATION);
	forwardForceObserver.init(EventTypes::FORWARD_FORCE);

}

//
//	Need to fix this, as it can be misinterrupted as moving the entity to
//  a specific location in world space
//
void Entity::move(float x, float y, float z)
{
	//btVector3 temp(x, y, z);

	//position += temp;
}
void Entity::move(const btVector3 &newPos)
{
	//position += newPos;
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
/*
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
*/
}

btScalar* Entity::getGLMatrix()
{
	//btScalar* glMat = new btScalar[16];
	//btTransform trans;// = new btTransform();
	//physicsObject->getMotionState()->getWorldTransform(trans);

	btTransform transM = physicsObject->getWorldTransform();

	transM.getOpenGLMatrix(glMatrix);

	//trans.getOpenGLMatrix(glMat);

	return glMatrix;
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



// CODE WILL PROBBALY BE PHASED OUT OVER A COUPLE OF COMMITS
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
/*
bool Entity::loadObj(char* filename, btScalar &mass, btTransform &trans)
{
	//printf("loading...\n");
	
	if(isInit)
	{
		if(renderObject != NULL)
		{
			aiReleaseImport(renderObject);
		}

		if(physicsObject != NULL)
		{
			delete physicsObject;
		}

		//renderObject = aiImportFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

		renderObject = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
		
		// Generate the physics representation
		if(renderObject->HasMeshes())
		{
			btCollisionShape* objShape;
			btVector3 inertia(0,0,0);
				
			// generate triangle mesh for bullet
			btTriangleMesh* bttm = new btTriangleMesh();
			
			for(unsigned int i = 0; i < renderObject->mNumMeshes; i++)
			{
				aiMesh* mesh = renderObject->mMeshes[i];
			
				for(unsigned int j = 0; j < mesh->mNumFaces; j++)
				{
					
					aiFace* face = &mesh->mFaces[j];	// get a face
			
					int index0 = face->mIndices[0];
					int index1 = face->mIndices[1];
					int index2 = face->mIndices[2];

					aiVector3D* v0 = &mesh->mVertices[index0];
					aiVector3D* v1 = &mesh->mVertices[index1];
					aiVector3D* v2 = &mesh->mVertices[index2];

					btVector3 btv0 = btVector3(v0->x, v0->y, v0->z);
					btVector3 btv1 = btVector3(v1->x, v1->y, v1->z);
					btVector3 btv2 = btVector3(v2->x, v2->y, v2->z);

					bttm->addTriangle(btv0, btv1, btv2, false);	// addTriangle(v0, v1, v2, removeDupes)
				}
			}

			// static rigid body
			if(mass == 0.f)
			{
				objShape = new btBvhTriangleMeshShape(bttm, true, true);
			}
			// dynamic rigid body
			else
			{

				objShape = new btBoxShape(btVector3(2.5, 2.5, 5));
				//objShape = new btConvexTriangleMeshShape(bttm, true);

				objShape->calculateLocalInertia(mass, inertia);
			}			

			// btDefaultMotionState(orientiation matrix, position)
			// need to pass orientation and position at the start
			btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

			// entRigidBodyCI(mass, motion state, collision shape, inertia);
			// need to update the mass to make it as a variable?
			btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(mass,entMotionState,objShape,inertia);

			physicsObject = new btRigidBody(entRigidBodyCI);

			return true;
		}
	}

	return false;	// render and physics representations failed to init
}
*/

bool Entity::initRenderObject(char* filename)
{
	std::ifstream fileCheck(filename);

	// if the file exists
	if(fileCheck)
	{
		// if a previous model was loaded
		if(renderObject != NULL)
		{
			aiReleaseImport(renderObject);
		}
		
		// load the file
		renderObject = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);

		return true;
	}
			
	return false;
}



/*
*	debug console output
*/
void Entity::debug()
{	
/*
	printf("-----------------------------------------------\n");
	printf("Pos: (%f, %f, %f)\n", position.x(), position.y(), position.z());
	printf("\n");
	printf("Tan: (%f, %f, %f)\n", tangent.x(), tangent.y(), tangent.z());
	printf("Nor: (%f, %f, %f)\n", normal.x(), normal.y(), normal.z());
	printf("Bin: (%f, %f, %f)\n", binormal.x(), binormal.y(), binormal.z());
*/
}


/*
*	Returns a string presentation of the entity
*
*	Return:
*		string representation of this entity
*		ie. position, tangent, normal, binormal.
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