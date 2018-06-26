#pragma once

#include "Conventions.hpp"

class World;
struct WorldParameterSet;

class ReliefGenerator
{
	static void SetupBuffers(World&, Size);

	static void LiftTerrain(Float2, Float, Size);

public:
	static void Generate(World&, Size);
};
