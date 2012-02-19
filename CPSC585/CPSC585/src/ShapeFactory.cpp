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
btCollisionShape* ShapeFactory::createPlane(btVector3& normal)
{
	btCollisionShape* planeShape = new btStaticPlaneShape(normal, 0);

	return planeShape;
}

btCollisionShape* ShapeFactory::createStaticTriangleMesh(const aiScene *renderObject)
{
	btCollisionShape* objShape;

	// Generate the physics representation
	if(renderObject->HasMeshes())
	{
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

		objShape = new btBvhTriangleMeshShape(bttm, true, true);
	}

	return objShape;

}

btCollisionShape* ShapeFactory::createDynamicTriangleMesh(const aiScene *renderObject)
{
	btCollisionShape* objShape;

	// Generate the physics representation
	if(renderObject->HasMeshes())
	{
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

		objShape = new btConvexTriangleMeshShape(bttm, true);
	}

	return objShape;

}