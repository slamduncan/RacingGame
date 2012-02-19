#include "ShaderManager.h"

ShaderManager::ShaderManager(void)
{
}

ShaderManager::~ShaderManager(void)
{
}

void ShaderManager::addShader(std::string sName, std::string vert, std::string frag, std::vector<std::string> paramList)
{
	Shader sh(vert.c_str(), frag.c_str());

	


}