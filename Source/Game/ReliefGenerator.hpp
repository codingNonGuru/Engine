#pragma once

#include "Conventions.hpp"

class World;
struct WorldParameterSet;
class DataBuffer;

class ReliefGenerator
{
	static Map <DataBuffer*, Word> modelBuffers_;

	static void SetupBuffers(World&);

	static void LiftTerrain(Float2, Float, Size);

public:
	static void Generate(World&);

	static void GenerateModel(World&);

	static DataBuffer* GetFinalBuffer();

	static DataBuffer* GetModelBuffer(Word);
};
