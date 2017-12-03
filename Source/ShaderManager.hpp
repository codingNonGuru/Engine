#pragma once

#include "Conventions.hpp"

class Shader;
class ShaderFile;

class ShaderManager
{
private:
	static Array <ShaderFile> shaderFiles_;

	static Map <Shader, LongWord> shaders_;

public:
	static void LoadShaders();

	static Map <Shader, LongWord> &GetShaderMap() {return shaders_;}
};
