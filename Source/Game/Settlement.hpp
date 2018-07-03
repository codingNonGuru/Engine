#pragma once

#include "Conventions.hpp"

class Tile;
class World;

class Settlement
{
	static World* world_;

	Tile* tile_;

	Position2 position_;

	int latitude_, longitude_;

	int population_;

	void EvaluateFertility();

public:
	static void SetWorld(World*);

	void Initialize(Position2);

	int GetPopulation() const;
};
