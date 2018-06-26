#include "Game/World.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/WorldParameterSet.hpp"

World::World() {}

World::World(File* file)
{
}

World::World(const WorldParameterSet& parameterSet)
{
	WorldGenerator::Generate(*this, parameterSet);
}

Grid <Tile> & World::GetTiles()
{
	return tiles_;
}

Size World::GetSize()
{
	return size_;
}

void World::SetSize(Size size)
{
	size_ = size;
}
