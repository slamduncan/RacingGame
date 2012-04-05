#include "Track.h"

Track::Track()
{
	lowest = btScalar(0.f);
}

Track::~Track()
{

}

/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Track::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	//objShape = new btBvhTriangleMeshShape(bttm, true, true);
	
	// there is a collision shape
	if(cShape != NULL)
	{
		btDefaultMotionState* entMotionState = new btDefaultMotionState(trans);

		// entRigidBodyCI(mass, motion state, collision shape, inertia);
		// need to update the mass to make it as a variable?
		btRigidBody::btRigidBodyConstructionInfo entRigidBodyCI(0,entMotionState,cShape,btVector3(0, 0, 0));

		physicsObject = new btRigidBody(entRigidBodyCI);

		return true;
	}
	return false;
}

void Track::initObservers()
{

}

void Track::lowestPoint()
{
	if(renderObject)
	{
		
		for(unsigned int i = 0; i < renderObject->mNumMeshes; i++)
		{
			const aiMesh* mesh = renderObject->mMeshes[i];

			for(unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				aiVector3D vertex = mesh->mVertices[v];

				if(vertex.y < lowest)
				{
					lowest = vertex.y;
				}
			}
		}
	}
}