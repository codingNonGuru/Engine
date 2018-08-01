#pragma once

#include "Conventions.hpp"

class Shader;
class DataBuffer;

typedef float ContrastStrength;
typedef float ContrastThreshold;

class Perlin
{
	static Shader* shader_;

	static Map <DataBuffer> buffers_;

	static DataBuffer* targetBuffer_;

	static bool isInitialized_;

	static void Initialize();

public:
	static void Destroy();

	static void SetTargetBuffer(DataBuffer*);

	static DataBuffer* Generate(Size, Range, float, ContrastThreshold = 0.5f, ContrastStrength = 1.0f);

	static void Download(container::Matrix*);

	static DataBuffer* GetResultBuffer();
};
