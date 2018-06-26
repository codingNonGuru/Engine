#pragma once

#include "Conventions.hpp"

class File;
struct WorldParameterSet;
class Tile;

class World
{
	Size size_;

	Grid <Tile> tiles_;

public:
	World();

	World(File*);

	World(const WorldParameterSet&);

	Grid <Tile> & GetTiles();

	Size GetSize();

	void SetSize(Size);
};
