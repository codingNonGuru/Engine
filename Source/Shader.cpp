#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "GL/glew.h"

#include "Shader.hpp"
#include "ShaderParser.hpp"
#include "ShaderUtility.hpp"
#include "Texture.hpp"

// ----- SHADER Definitions

Shader::Shader(const char* name, Array <ShaderFile*> &files)
{
	Initialize(name, files);
}

void Shader::Initialize(const char* name, Array <ShaderFile*> &shaderFiles)
{
	shaderFiles_ = &shaderFiles;
	name_.Add(name);

	Index index = 0;
	for(auto shaderFile = shaderFiles.GetStart(); shaderFile != shaderFiles.GetEnd(); ++shaderFile)
	{
		Compile(index, (GLenum)(*shaderFile)->GetType());
	}

	Link();

	Parse();
}

void Shader::Bind()
{
	glUseProgram(key_);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

bool Shader::BindTexture(Texture* texture, const char* identifier)
{
	auto binding = textureBindings_.Get(identifier);
	if(!binding)
	{
		std::cout<<"Texture identifier "<<identifier<<" is not valid.\n";
		return false;
	}

	glUniform1i(binding->GetLocation(), binding->GetUnitIndex());
	glActiveTexture(GL_TEXTURE0 + binding->GetUnitIndex());
	texture->Bind();

	return true;
}

void Shader::Compile(int index, GLenum shaderType)
{
	GLuint key = glCreateShader(shaderType);
	shaderKeys_[index] = key;

	auto shaderFile = *shaderFiles_->Get(index);

	std::ifstream fileStream(shaderFile->GetPath());
	std::string fileContent = "";
	std::string line;
	while(std::getline(fileStream, line))
		fileContent += line + "\n";

	fileStream.close();
	const char* fC = fileContent.c_str();

	glShaderSource(key, 1, &fC, nullptr);
	glCompileShader(key);

	GLint isCompiled = 0;
	glGetShaderiv(key, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(key, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar errorLog[maxLength];
		glGetShaderInfoLog(key, maxLength, &maxLength, &errorLog[0]);
		for(int i = 0; i < maxLength; ++i)
		{
			std::cout<<errorLog[i];
		}
		glDeleteShader(key);
		return;
	}
}

void Shader::Link()
{
	key_ = glCreateProgram();

	for(int i = 0; i < shaderFiles_->GetSize(); ++i)
	{
		glAttachShader(key_, shaderKeys_[i]);
	}

	glLinkProgram(key_);
	glValidateProgram(key_);

	GLint isLinked = 0;
	glGetProgramiv(key_, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(key_, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar infoLog[maxLength];
		glGetProgramInfoLog(key_, maxLength, &maxLength, &infoLog[0]);
		for(int i = 0; i < maxLength; ++i)
		{
			std::cout<<infoLog[i];
		}
		glDeleteProgram(key_);
		for(int i = 0; i < shaderFiles_->GetSize(); ++i)
		{
			glDeleteShader(shaderKeys_[i]);
		}
		return;
	}

	for(int i = 0; i < shaderFiles_->GetSize(); ++i)
	{
		glDetachShader(key_, shaderKeys_[i]);
	}
}

void Shader::SetConstant(void* value, const char* identifier)
{
	auto binding = constantBindings_.Get(LongWord(identifier));
	if(binding == nullptr)
	{
		std::cout<<"Constant identifier "<<identifier<<" is not valid.\n";
		return;
	}

	switch(binding->GetType())
	{
	case AttributeElementTypes::FLOAT_1:
		glUniform1fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_2:
		glUniform2fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_3:
		glUniform3fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::FLOAT_4:
		glUniform4fv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_1:
		glUniform1iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_2:
		glUniform2iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_3:
		glUniform3iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::INTEGER_4:
		glUniform4iv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_1:
		glUniform1uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_2:
		glUniform2uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_3:
		glUniform3uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::UINTEGER_4:
		glUniform4uiv(binding->GetLocation(), 1, value);
		break;
	case AttributeElementTypes::MATRIX_3:
		glUniformMatrix3fv(binding->GetLocation(), 1, GL_FALSE, value);
		break;
	case AttributeElementTypes::MATRIX_4:
		glUniformMatrix4fv(binding->GetLocation(), 1, GL_FALSE, value);
		break;
	}
}

void Shader::Update()
{
	/*glDeleteProgram(key_);
	for(int i = 0; i < shaderFiles_->getSize(); ++i)
	{
		glDeleteShader(shaderKeys_[i]);
	}

	if(shaderFiles_.getSize() == 1)
	{
		Compile(0, GL_COMPUTE_SHADER);
	}
	else if(shaderFiles_.getSize() == 2)
	{
		Compile(0, GL_VERTEX_SHADER);
		Compile(1, GL_FRAGMENT_SHADER);
	}
	else if(shaderFiles_.getSize() == 3)
	{
		Compile(0, GL_VERTEX_SHADER);
		Compile(1, GL_FRAGMENT_SHADER);
		Compile(2, GL_GEOMETRY_SHADER);
	}
	Link();*/
}

GLuint Shader::GetTextureLocation(const char* textureName)
{
	return glGetUniformLocation(key_, textureName);
}

void Shader::Parse()
{
	auto parser = new ShaderParser(this);

	auto textureExpressions = parser->FetchTextures();

	textureBindings_.Initialize(textureExpressions.GetSize());
	Index index = 0;
	for(auto expression = textureExpressions.GetStart(); expression != textureExpressions.GetEnd(); ++expression, ++index)
	{
		Index location = glGetUniformLocation(key_, expression->name_);

		*textureBindings_.Allocate(expression->name_) = TextureBinding(index, location);
	}

	auto constantExpressions = parser->FetchConstants();

	constantBindings_.Initialize(constantExpressions.GetSize());
	index = 0;
	for(auto expression = constantExpressions.GetStart(); expression != constantExpressions.GetEnd(); ++expression, ++index)
	{
		Index location = glGetUniformLocation(key_, expression->name_);

		auto& typeName = expression->typeName_;
		AttributeElementTypes type;
		if(CompareMemory(typeName, "int", 3) == 0)
			type = AttributeElementTypes::INTEGER_1;
		else if(CompareMemory(typeName, "ivec2", 5) == 0)
			type = AttributeElementTypes::INTEGER_2;
		else if(CompareMemory(typeName, "ivec3", 5) == 0)
			type = AttributeElementTypes::INTEGER_3;
		else if(CompareMemory(typeName, "ivec4", 5) == 0)
			type = AttributeElementTypes::INTEGER_4;
		else if(CompareMemory(typeName, "uint", 4) == 0)
			type = AttributeElementTypes::UINTEGER_1;
		else if(CompareMemory(typeName, "uvec2", 5) == 0)
			type = AttributeElementTypes::UINTEGER_2;
		else if(CompareMemory(typeName, "uvec3", 5) == 0)
			type = AttributeElementTypes::UINTEGER_3;
		else if(CompareMemory(typeName, "uvec4", 5) == 0)
			type = AttributeElementTypes::UINTEGER_4;
		else if(CompareMemory(typeName, "float", 5) == 0)
			type = AttributeElementTypes::FLOAT_1;
		else if(CompareMemory(typeName, "vec2", 4) == 0)
			type = AttributeElementTypes::FLOAT_2;
		else if(CompareMemory(typeName, "vec3", 4) == 0)
			type = AttributeElementTypes::FLOAT_3;
		else if(CompareMemory(typeName, "vec4", 4) == 0)
			type = AttributeElementTypes::FLOAT_4;
		else if(CompareMemory(typeName, "mat3", 4) == 0)
			type = AttributeElementTypes::MATRIX_3;
		else if(CompareMemory(typeName, "mat4", 4) == 0)
			type = AttributeElementTypes::MATRIX_4;

		*constantBindings_.Allocate(expression->name_) = ConstantBinding(location, type);
	}

	delete parser;
}

Shader::~Shader(void) {}

// ----- SHADER MAP Definitions

ShaderMap::ShaderMap(int mapSize)
{
	shaders_.Initialize(mapSize);
}

void ShaderMap::Initialize(int mapSize)
{
	shaders_.Initialize(mapSize);
}

bool ShaderMap::Add(const char* shaderName, const char* firstShaderPath, const char* secondShaderPath, const char* thirdShaderPath)
{
	/*auto shader = shaders_.Allocate(ShortWord(shaderName));
	if(shader == nullptr)
		return false;

	if(thirdShaderPath != nullptr)
		shader->Initialize(firstShaderPath, secondShaderPath, thirdShaderPath);
	else if(secondShaderPath != nullptr)
		shader->Initialize(firstShaderPath, secondShaderPath);
	else
		shader->Initialize(firstShaderPath);*/

	return true;
}

void ShaderMap::Use(const char* shaderName)
{
	auto shader = shaders_.Get(ShortWord(shaderName));
	if(shader == nullptr)
		return;

	shader->Bind();
}

void ShaderMap::Unuse(const char* shaderName)
{
	auto shader = shaders_.Get(ShortWord(shaderName));
	if(shader == nullptr)
		return;

	shader->Unbind();
}

GLuint ShaderMap::GetTextureLocation(const char* shaderName, const char* textureName)
{
	auto shader = shaders_.Get(ShortWord(shaderName));
	if(shader == nullptr)
		return 0;

	return shader->GetTextureLocation(textureName);
}

Shader& ShaderMap::Get(const char* shaderName)
{
	return *shaders_.Get(ShortWord(shaderName));
}

Shader& ShaderMap::operator [](const char* shaderName)
{
	return *shaders_.Get(ShortWord(shaderName));
}

#pragma endregion ShaderMap
