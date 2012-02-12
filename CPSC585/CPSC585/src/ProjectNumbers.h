//#define TIXML_USE_STL
#ifndef PROJECTNUMBERS_H
#define PROJECTNUMBERS_H

#include <fstream>

#include "tinyxml.h"
#include "tinystr.h"
#include "LinearMath/btVector3.h"


class ProjectNumbers {

public:
	int* CONTROLLER_Turning;
	int test;
	float cModifier;
	float kModifier;
	btVector3 gravity;

	ProjectNumbers(){test = 0; CONTROLLER_Turning = &test;}


	void readVariablesIn(){
		readPhysics();
		readController();
	}

	void readPhysics();
	void readController();

};
#endif