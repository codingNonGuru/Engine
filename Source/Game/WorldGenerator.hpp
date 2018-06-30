#pragma once

#include "Conventions.hpp"
#include "Delegate.hpp"

#include "Game/Types.hpp"

class World;
struct WorldParameterSet;
class Texture;

class WorldGenerator
{
	static const WorldParameterSet* parameterSet_;

	static Map <Texture*, WorldSizeOptions> reliefTextures_;

	static Map <Texture*, WorldSizeOptions> biomeTextures_;

public:
	static Delegate OnWorldGenerated_;

	static Delegate OnReliefGenerated_;

	static Delegate OnBiomeGenerated_;

	static void Generate(World&, const WorldParameterSet&);

	static Texture* GetReliefPreview();

	static Texture* GetBiomePreview();
};
