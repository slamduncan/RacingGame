#pragma once

#include <map>
#include <string>
#include <vector>
#include "Shader.h"

typedef std::map<std::string, Shader> ShaderMap;

class ShaderManager
{
private:
	ShaderMap smap;

public:
	ShaderManager(void);
	~ShaderManager(void);

	void addShader(std::string sName, std::string vert, std::string frag, std::vector<std::string> paramList);
};
