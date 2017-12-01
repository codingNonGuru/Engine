#include <stdio.h>

#include "GL/glew.h"

#include "ShaderParser.hpp"
#include "Shader.hpp"
#include "ShaderUtility.hpp"

bool ShaderParser::isTracing_ = true;

ShaderParser::ShaderParser(Shader* shader)
{
	if(isTracing_)
	{
		std::cout<<"Parsing Shader: "<<shader<<"\n";
	}

	shader_ = shader;

	Setup();

	Parse();

	if(isTracing_)
	{
		for(auto expression = samplerExpressions_.getStart(); expression != samplerExpressions_.getEnd(); ++expression)
		{
			std::cout<<"Texture has Identifier <"<<expression->name_<<">\n";
		}
		std::cout<<"\n";

		for(auto expression = constantExpressions_.getStart(); expression != constantExpressions_.getEnd(); ++expression)
		{
			std::cout<<"Constant has Identifier <"<<expression->name_<<"> & Type Name <"<<expression->typeName_<<">\n";
		}
		std::cout<<"\n";
	}
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

	LocateConstants();

	delete[] shaderCode;
	fclose(file);
}

void ShaderParser::Setup()
{
	int maximumSamplerCount = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maximumSamplerCount);

	samplerExpressions_.initialize(maximumSamplerCount);

	constantExpressions_.initialize(32);
}

Index FindNextSymbol(char *shaderCodeIterator, char symbol)
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
		const char* typeName = "sampler2D";
		if(CompareMemory(character, typeName, GetStringLength(typeName)) != 0)
			continue;

		character += GetStringLength(typeName);

		character = shaderCode + FindNextSymbol(character, ' ');

		Index identifierPosition = JumpSymbol(character, ' ');

		character = shaderCode + identifierPosition;

		Index semicolonPosition = FindNextSymbol(character, ';');

		LongWord identifier;
		identifier.Add(character, semicolonPosition - identifierPosition);

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

		*samplerExpressions_.allocate() = Expression(identifierPosition, identifier);
	}
}

void ShaderParser::LocateConstants()
{
	for(auto character = shaderCode; character != shaderCode + fileSize; ++character)
	{
		const char* labelName = "uniform";
		if(CompareMemory(character, labelName, GetStringLength(labelName)) != 0)
			continue;

		character += GetStringLength(labelName);

		character = shaderCode + FindNextSymbol(character, ' ');

		Index typeStartPosition = JumpSymbol(character, ' ');

		character = shaderCode + typeStartPosition;

		if(*character == 's')
			continue;

		Index typeEndPosition = FindNextSymbol(character, ' ');

		LongWord typeName;
		typeName.Add(character, typeEndPosition - typeStartPosition);

		character = shaderCode + typeEndPosition;

		Index identifierStartPosition = JumpSymbol(character, ' ');

		character = shaderCode + identifierStartPosition;

		Index semiColonPosition = FindNextSymbol(character, ';');

		LongWord identifier;
		identifier.Add(character, semiColonPosition - identifierStartPosition);

		bool hasFound = false;
		for(auto expression = constantExpressions_.getStart(); expression != constantExpressions_.getEnd(); ++expression)
		{
			if(expression->position_ != identifierStartPosition)
				continue;

			hasFound = true;
		}

		if(hasFound)
		{
			character = shaderCode + semiColonPosition;
			continue;
		}

		*constantExpressions_.allocate() = Expression(identifierStartPosition, identifier, typeName);
	}

	std::cout<<"\n";
}

Array <ShaderParser::Expression> &ShaderParser::FetchTextures()
{
	return samplerExpressions_;
}

Array <ShaderParser::Expression> &ShaderParser::FetchConstants()
{
	return constantExpressions_;
}

ShaderParser::~ShaderParser()
{
	samplerExpressions_.Destroy();

	constantExpressions_.Destroy();
}
