#pragma once

#include "Conventions.hpp"

class Shader;
class DataBuffer;

class Perlin
{
	static Shader* shader_;

	static Map <DataBuffer, LongWord> buffers_;

	static bool isInitialized_;

	static void Initialize();

public:
	static void Destroy();

	static DataBuffer* Generate(Size, Range, float, float, float, float);

	static void Download(container::Matrix*);

	static DataBuffer* GetResultBuffer();
};
