#include "ShapeFactory.h"

ShapeFactory::ShapeFactory()
{
}

ShapeFactory::~ShapeFactory()
{
}

btCollisionShape* ShapeFactory::createBox(btScalar &width, btScalar &height, btScalar &depth)
{
	btCollisionShape* boxShape = new btBoxShape(btVector3(width/2.0f, height/2.0f, depth/2.0f));

	return boxShape;
}

btCollisionShape* ShapeFactory::createSphere(btScalar & radius)
{
	btCollisionShape* sphereShape = new btSphereShape(radius);

	return sphereShape;
}
btCollisionShape* ShapeFactory::createPlane(btVector3 &point, btVector3& normal)
{
	btCollisionShape* planeShape = new btStaticPlaneShape(normal, 0);

	return planeShape;
}

void ShapeFactory::createTriangleMesh(aiScene *renderObject)
{
	
}