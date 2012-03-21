#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "LinearMath/btHashMap.h"
#include "assimp.h"	//c interface
#include "aiPostProcess.h"
#include "aiScene.h"

class ModelManager
{
	btHashMap<std::string, aiScene*> modelKeeper;

	ModelManager();
	~ModelManager();

	aiScene* loadModel(std::string name);
};
#endif