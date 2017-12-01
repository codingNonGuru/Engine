#pragma once

#include "Conventions.hpp"

class Shader;
class ShaderFile;

class ShaderParser
{
private:
	struct Expression
	{
		Index position_;

		LongWord name_;

		LongWord typeName_;

		Expression() {}

		Expression(Index position, LongWord name) {position_ = position; name_ = name;}

		Expression(Index position, LongWord name, LongWord typeName) {position_ = position; name_ = name; typeName_ = typeName;}
	};

	static bool isTracing_;

	Shader* shader_;

	Array <Expression> samplerExpressions_;

	Array <Expression> constantExpressions_;

	void Parse();

	void Parse(ShaderFile*);

	void Setup();

	void LocateSamplers();

	void LocateConstants();

public:
	ShaderParser() {}

	ShaderParser(Shader*);

	Array <Expression> &FetchTextures();

	Array <Expression> &FetchConstants();

	~ShaderParser();
};
