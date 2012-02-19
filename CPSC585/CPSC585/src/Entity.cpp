#include "Entity.h"
#include "EventSystemHandler.h"

Entity::Entity()
{
	physicsObject = NULL;
	renderObject = NULL;

	// we create a 16 element array for our transform
	glMatrix = new btScalar[16];
}


/*
*	Clean up
*/
Entity::~Entity()
{
	if(renderObject != NULL)
	{
		aiReleaseImport(renderObject);	// delete the render object
	}

	// if there is a physics object, clean up
	if(physicsObject != NULL)
	{
		delete physicsObject;	// delete the physics object
	}

	delete[] glMatrix;	// delete the gl transform matrix
}

// The following 3 functions have become obsolete
void Entity::move(float x, float y, float z)
{
}
void Entity::move(const btVector3 &newPos)
{
}
void Entity::rotate(const btVector3 &axis, int deg)
{
}
//

/*
*	Returns a pointer to the 16 element array for opengl matrix multiplication
*
*	Return:
*	pointer to 16 element array for opengl matrix
*/
btScalar* Entity::getGLMatrix()
{
	btTransform transM = physicsObject->getWorldTransform();
	transM.getOpenGLMatrix(glMatrix);
	return glMatrix;
}

/*
*	Returns the position in world space for this entity
*
*	Return:
*	center of mass of the entity in world space
*/
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
*	loads a 3d model for this entity
*	used for rendering.
*
*	Parameters: filename - the path the model is located at
*
*	Return:
*	Whether or not the model was loaded properly
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
		renderObject = aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs );

		return true;
	}
			
	return false;
}



/*
*	debug console output
*/
void Entity::debug()
{	

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