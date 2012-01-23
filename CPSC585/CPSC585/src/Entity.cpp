#include "Entity.h"

Entity::Entity()
{
	isInit = init();
	
	loadObj("../CPSC585/model/box.obj");
}

Entity::Entity(char* filename)
{
	if(filename != NULL)
	{
		//renderObject->load(filename);
		isInit = init();

		loadObj(filename);

	}
	else
	{
		renderObject = NULL;
	}
}

Entity::~Entity()
{
	if(renderObject != NULL)
	{
		delete renderObject;
	}
}


bool Entity::init()
{
	position = btVector3(0,0,0);
	
	tangent = btVector3(0, 0, 1);
	normal = btVector3(0, 1, 0);
	binormal = tangent.cross(normal);

	renderObject = new objLoader();

	return true;
}

void Entity::move(float x, float y, float z)
{
	btVector3 temp(x, y, z);

	position += temp;
}
void Entity::move(const btVector3 &newPos)
{
	position += newPos;
}

bool Entity::loadObj(char* filename)
{
	//printf("loading...\n");
	
	if(isInit)
	{
		int loaded = renderObject->load(filename);

		if(loaded)
		{
			return true;
		}
	}

	return false;
}

void Entity::debug()
{
	printf("-----------------------------------------------\n");
	printf("Pos: (%f, %f, %f)\n", position.x(), position.y(), position.z());
	printf("\n");
	printf("Tan: (%f, %f, %f)\n", tangent.x(), tangent.y(), tangent.z());
	printf("Nor: (%f, %f, %f)\n", normal.x(), normal.y(), normal.z());
	printf("Bin: (%f, %f, %f)\n", binormal.x(), binormal.y(), binormal.z());
}

std::string Entity::toString()
{
	std::stringstream ss;

	ss << "Pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")\n";

	return ss.str();
}