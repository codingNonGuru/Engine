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

	Float averageHeight_;

	Float landRatio_;

	Biome biome_;

	Settlement* settlement_;

	Settlement* domain_;

public:
	Tile();

	Position3 & GetPosition();

	ReliefTypes GetRelief();

	Float GetLandRatio();

	void SetLandRatio(Float);

	Float GetAverageHeight();

	void SetAverageHeight(Float);

	Biome & GetBiome();

	Settlement* GetSettlement();

	void SetSettlement(Settlement*);

	Settlement* GetDomain();

	void SetDomain(Settlement*);
};
