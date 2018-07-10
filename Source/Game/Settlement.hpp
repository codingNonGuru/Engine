#pragma once

#include "Conventions.hpp"
#include "Utility/Color.hpp"

class Tile;
class World;

class Settlement
{
	static World* world_;

	Tile* tile_;

	Position2 position_;

	int latitude_, longitude_;

	int population_;

	Color banner_;

	void EvaluateFertility();

public:
	static void SetWorld(World*);

	Settlement();

	void Initialize(Position2);

	int GetPopulation() const;

	Position2 GetPosition() const;

	float GetDistance(Settlement*) const;

	Color & GetBanner();
};
