#pragma once

#include "Conventions.hpp"
#include "Game/Types.hpp"

class Tile
{
	Position3 position_;

	ReliefTypes relief_;

public:
	Position3 & GetPosition();

	ReliefTypes & GetRelief();
};
