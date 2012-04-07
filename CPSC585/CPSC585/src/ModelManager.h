#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "LinearMath/btHashMap.h"
#include "assimp.h"	//c interface
#include "aiPostProcess.h"
#include "aiScene.h"

typedef std::map<std::string, const aiScene*, std::less<std::string>> ModelMap;

class ModelManager
{
	ModelMap modelman;
	std::vector<std::string> modelList;
public:
	ModelManager();
	~ModelManager();

	void loadAll();
	void cleanAll();
	const aiScene* getModel(std::string key);

	aiScene* loadModel(std::string name);
};
#endif