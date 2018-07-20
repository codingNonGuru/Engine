#pragma once

#include "Conventions.hpp"

class World;
struct WorldParameterSet;
class DataBuffer;

class ReliefGenerator
{
	static Map <DataBuffer*> modelBuffers_;

	static Map <Texture*> modelTextures_;

	static void SetupBuffers(World&);

	static void LiftTerrain(Float2, Float, Size);

public:
	static void Generate(World&);

	static void GenerateModel(World&);

	static DataBuffer* GetFinalBuffer();

	static DataBuffer* GetModelBuffer(Word);

	static Texture* GetModelTexture(Word);
};
