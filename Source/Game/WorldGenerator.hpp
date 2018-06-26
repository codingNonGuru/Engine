#pragma once

#include "Conventions.hpp"
#include "Delegate.hpp"

#include "Game/Types.hpp"

class World;
struct WorldParameterSet;
class Texture;

class WorldGenerator
{
	static WorldParameterSet* parameterSet_;

	static Map <Texture*, WorldSizeOptions> reliefTextures_;

public:
	static Delegate OnWorldGenerated_;

	static void Generate(World&, const WorldParameterSet&);

	static Texture* GetReliefPreview();
};
