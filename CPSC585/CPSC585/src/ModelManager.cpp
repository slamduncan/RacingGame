#include "ModelManager.h"

ModelManager::ModelManager()
{
	loadAll();
}

ModelManager::~ModelManager()
{
	cleanAll();
}

void ModelManager::loadAll()
{
	loadModel("model/Track1tri.lwo");
	loadModel("model/groundBox.lwo");
	loadModel("model/waypoint.obj");
	loadModel("model/skydome2.lwo");
	loadModel("model/rocket.lwo");
	loadModel("model/shield.lwo");
	loadModel("model/powerup.lwo");
	loadModel("model/mine.lwo");
	loadModel("model/nova.lwo");
	loadModel("model/slow.lwo");
	loadModel("model/ship1.lwo");
	loadModel("model/particle.lwo");
}
void ModelManager::cleanAll()
{
	for(unsigned int i = 0; i < modelList.size(); i++)
	{
		std::string key = modelList[i];

		const aiScene* temp = getModel(key);
		aiReleaseImport(temp);

		modelman.erase(key);
	}
}

const aiScene* ModelManager::getModel(std::string key)
{
	int counter = modelman.count(key);

	if(counter > 0)
	{
		return modelman.find(key)->second;
	}

	return NULL;
}

aiScene* ModelManager::loadModel(std::string name)
{
	const aiScene* scene;
	
	std::ifstream fileCheck(name.c_str());

	// if the file exists
	if(fileCheck)
	{
		
		// load the file
		scene = aiImportFile(name.c_str(), 
			aiProcess_CalcTangentSpace       |
			aiProcess_Triangulate            |
			//aiProcess_JoinIdenticalVertices  |
			aiProcess_GenSmoothNormals |
			aiProcess_ImproveCacheLocality |
			aiProcess_GenUVCoords |
			aiProcess_TransformUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
			/*aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs*/ );

		modelman.insert(std::pair<std::string, const aiScene*>(name, scene));
		//modelKeeper.insert(name, scene);
		modelList.push_back(name);
	}

	return NULL;
}