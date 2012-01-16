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

bool Entity::loadObj(char* filename)
{
	printf("loading...\n");
	
	if(isInit)
	{
		int loaded = renderObject->load(filename);

		if(loaded)
		{
			int num = renderObject->materialCount;

			printf("mat count is %d\n", num);

			
			return true;
		}
	}

	return false;
}
