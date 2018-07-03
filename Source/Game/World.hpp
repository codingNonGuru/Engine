#pragma once

#include "Conventions.hpp"

class File;
struct WorldParameterSet;
class Tile;
class Settlement;

class World
{
	Size size_;

	Grid <Tile> tiles_;

	Array <Settlement> settlements_;

public:
	World();

	World(File*);

	World(const WorldParameterSet&);

	Grid <Tile> & GetTiles();

	Size GetSize();

	void SetSize(Size);

	Array <Settlement> & GetSettlements();
};
