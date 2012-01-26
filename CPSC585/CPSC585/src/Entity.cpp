#include "Entity.h"

Entity::Entity()
{
	renderObject = NULL;
	physicsObject = NULL;
	isInit = init();
	
	loadObj("../CPSC585/model/box.obj");
}

/*
*	Creates an entity and tries to load the obj model passed as a parameter
*
*	Parameter:
*	filename - location of the obj file
*/
Entity::Entity(char* filename)
{
	if(filename != NULL)
	{
		isInit = init();

		loadObj(filename);
	}
	else
	{
		renderObject = NULL;
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
	
	cf = btMatrix3x3();
	cf.setIdentity();
	
	tangent = btVector3(0, 0, 1);
	normal = btVector3(0, 1, 0);
	binormal = normal.cross(tangent);

	loaded = 0;
	//renderObject = new objLoader();

	return true;
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

/*
*	Loads a given obj model to this entity.
*
*	Parameters:
*	filename - location of the model
*
*	Return:
*	whether or not the obj loaded sucessfully
*/
bool Entity::loadObj(char* filename)
{
	//printf("loading...\n");
	
	if(isInit)
	{
		if(loaded)
		{
			delete renderObject;
		}
		
		renderObject = new objLoader();

		// returns 0 if fails to load
		loaded = renderObject->load(filename);

		if(loaded)
		{
			// generate triangle mesh for bullet
			btTriangleMesh* bttm = new btTriangleMesh();
			
			// generate the triangle mesh for bullet
			for(int i = 0; i < renderObject->faceCount; i++)
			{
				obj_face* face = renderObject->faceList[i];	// get a triangle

				obj_vector* vert0 = renderObject->vertexList[face->vertex_index[0]];
				obj_vector* vert1 = renderObject->vertexList[face->vertex_index[1]];
				obj_vector* vert2 = renderObject->vertexList[face->vertex_index[2]];

				btVector3 btv0 = btVector3(vert0->e[0], vert0->e[1], vert0->e[2]);
				btVector3 btv1 = btVector3(vert1->e[0], vert1->e[1], vert1->e[2]);
				btVector3 btv2 = btVector3(vert2->e[0], vert2->e[1], vert2->e[2]);
				
				bttm->addTriangle(btv0, btv1, btv2, false);
			}

			physicsObject = new btBvhTriangleMeshShape(bttm, true, true);

			return true;
		}
	}

	return false;
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
	std::stringstream ss;

	ss << "Pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")\n";
	ss << "Tangent: (" << tangent.x() << ", " << tangent.y() << ", " << tangent.z() << ")\n";
	ss << "Normal: (" << normal.x() << ", " << normal.y() << ", " << normal.z() << ")\n";
	ss << "Binormal: (" << binormal.x() << ", " << binormal.y() << ", " << binormal.z() << ")";

	return ss.str();
}