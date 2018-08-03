#include "Game/World.hpp"
#include "Game/WorldGenerator.hpp"
#include "Game/WorldParameterSet.hpp"
#include "Game/Settlement.hpp"

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

Array <Settlement> & World::GetSettlements()
{
	return settlements_;
}

Float World::GetHeight(Float x, Float y)
{
	return 0.0f;
}

Grid <Float> & World::GetHeightMap()
{
	return heightMap_;
}

void World::SetDetailResolution(int detailResolution)
{
	detailResolution_ = detailResolution;
}
