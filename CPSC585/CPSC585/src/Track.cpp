#include "Track.h"

Track::Track()
{
	indicator = NULL;
	lowest = btScalar(0.f);
}

Track::~Track()
{
	if(indicator)
	{
		aiReleaseImport(indicator);
	}
}

/*
*	Initializes the Physics representation of the object based on a collision shape, mass, and transform
*
*	Note: collision shape must be created outside of the function for now
*/
bool Track::initPhysicsObject(btCollisionShape* cShape, btScalar &mass, btTransform &trans)
{
	
	if (indicator == NULL)
	{
		indicator = aiImportFile("model/indicator.lwo", 
			aiProcess_CalcTangentSpace       |
			aiProcess_Triangulate            |
			//aiProcess_JoinIdenticalVertices  |
			aiProcess_GenSmoothNormals |
			aiProcess_ImproveCacheLocality |
			//aiProcess_GenUVCoords |
			//aiProcess_TransformUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
			/*aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs*/ );
	}
	
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