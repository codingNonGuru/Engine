#pragma once

#include "Conventions.hpp"

class World;
struct WorldParameterSet;
class DataBuffer;

class ReliefGenerator
{
	static void SetupBuffers(World&);

	static void LiftTerrain(Float2, Float, Size);

public:
	static void Generate(World&);

	static DataBuffer* GetFinalBuffer();
};
