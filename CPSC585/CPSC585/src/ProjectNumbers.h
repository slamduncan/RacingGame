//#define TIXML_USE_STL
#ifndef PROJECTNUMBERS_H
#define PROJECTNUMBERS_H

#include <fstream>

#include "tinyxml.h"
#include "tinystr.h"
#include "LinearMath/btVector3.h"


class ProjectNumbers {

public:
	struct controller{		
		float rotateModifier;
	} controllerInfo;

	struct physics{
		float cModifier;
		float kModifier;
		btVector3 gravity;
	} physicsInfo;

	struct AI{
		float rotateModifier;
	} aiInfo;

	ProjectNumbers(){readVariablesIn();}


	void readVariablesIn(){
		readPhysics();
		readController();
	}

	void readPhysics();
	void readController();
	void readAI();

};
#endif