#pragma once

#include "Conventions.hpp"

class Shader;
class ShaderFile;

class ShaderParser
{
private:
	struct SamplerExpression
	{
		Index position_;

		LongWord name_;

		SamplerExpression() {}

		SamplerExpression(Index position, LongWord name) {position_ = position; name_ = name;}
	};

	Shader* shader_;

	Array <SamplerExpression> samplerExpressions_;

	void Parse();

	void Parse(ShaderFile*);

	void Setup();

	void LocateSamplers();

public:
	ShaderParser() {}

	ShaderParser(Shader*);

	const Array <LongWord> *FetchTextures();
};
