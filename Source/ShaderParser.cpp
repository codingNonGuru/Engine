#include <stdio.h>

#include "GL/glew.h"

#include "ShaderParser.hpp"
#include "Shader.hpp"

ShaderParser::ShaderParser(Shader* shader)
{
	shader_ = shader;

	Setup();

	Parse();
}

void ShaderParser::Parse()
{
	if(shader_ == nullptr)
		return;

	auto shaderFiles = shader_->GetShaderFiles();
	for(auto shaderFile = shaderFiles.getStart(); shaderFile != shaderFiles.getEnd(); ++shaderFile)
	{
		Parse(shaderFile);
	}
}

int fileSize = 0;

char* shaderCode = nullptr;

void ShaderParser::Parse(ShaderFile* shaderFile)
{
	FILE* file = fopen(shaderFile->GetPath(), "r");
	if(!file)
		return;

	fseek(file, 0 , SEEK_END);
	fileSize = ftell(file);
	rewind(file);

	shaderCode = new char[fileSize];
	fread(shaderCode, 1, fileSize, file);

	LocateSamplers();

	delete[] shaderCode;
	fclose(file);
}

void ShaderParser::Setup()
{
	int maximumSamplerCount = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maximumSamplerCount);

	samplerExpressions_.initialize(maximumSamplerCount);
}

Index LocateNextSymbol(char *shaderCodeIterator, char symbol)
{
	for(auto character = shaderCodeIterator; character != shaderCode + fileSize; ++character)
	{
		if(*character == symbol)
			return character - shaderCode;
	}
}

Index JumpSymbol(char* shaderCodeIterator, char symbol)
{
	for(auto character = shaderCodeIterator; character != shaderCode + fileSize; ++character)
	{
		if(*character == symbol)
			continue;

		return character - shaderCode;
	}
}

void ShaderParser::LocateSamplers()
{
	for(auto character = shaderCode; character != shaderCode + fileSize; ++character)
	{
		if(*character != 's')
			continue;

		const char* typeName = "sampler";
		if(strcmp(character, typeName) != 0)
			continue;

		character += strlen(typeName);

		character = shaderCode + LocateNextSymbol(character, ' ');

		Index identifierPosition = JumpSymbol(character, ' ');

		character = shaderCode + identifierPosition;

		Index semicolonPosition = LocateNextSymbol(character, ';');

		LongWord identifier;
		CopyMemory(identifier, character, semicolonPosition - identifierPosition);

		bool hasFound = false;
		for(auto expression = samplerExpressions_.getStart(); expression != samplerExpressions_.getEnd(); ++expression)
		{
			if(expression->position_ != identifierPosition)
				continue;

			hasFound = true;
		}

		if(hasFound)
		{
			character = shaderCode + semicolonPosition;
			continue;
		}

		*samplerExpressions_.allocate() = SamplerExpression(identifierPosition, identifier);
	}
}

const Array <LongWord> *ShaderParser::FetchTextures()
{
	int textureCount = samplerExpressions_.getSize();
	Array <LongWord> *textures = new Array <LongWord> (textureCount);

	for(int i = 0; i < textureCount; ++i)
	{
		*textures->allocate() = samplerExpressions_.get(i)->name_;
	}

	return textures;
}
