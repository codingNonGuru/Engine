#pragma once

#include "Conventions.hpp"

class Shader;
class DataBuffer;

class Perlin
{
	static Shader* shader_;

	static Map <DataBuffer, LongWord> buffers_;

	static bool isInitialized_;

public:
	static void Initialize();

	static void Destroy();

	static void Generate(Size, Range, float, float, float, float);

	static void Download(container::Matrix*);

	static DataBuffer* GetResultBuffer();
};
