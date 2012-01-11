#include "Entity.h"

Entity::Entity()
{
	position = Point();

	tangent = Point(0, 0, -1);
	normal = Point(0, 0, 1);
	binormal = crossProduct(tangent, normal);
}




Entity::~Entity()
{
}
