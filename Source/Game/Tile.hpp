#pragma once

#include "Conventions.hpp"
#include "Game/Types.hpp"

class Settlement;

struct Biome
{
	float Productivity_;

	float ForestCover_;
};

class Tile
{
	Position3 position_;

	ReliefTypes relief_;

	Biome biome_;

	Settlement* settlement_;

public:
	Tile();

	Position3 & GetPosition();

	ReliefTypes & GetRelief();

	Biome & GetBiome();

	Settlement* GetSettlement();

	void SetSettlement(Settlement*);
};
