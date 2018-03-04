#include "Game/WorldGenerator.hpp"
#include "Game/ReliefGenerator.hpp"

void WorldGenerator::Generate(World& world, const WorldParameterSet& parameterSet)
{
	ReliefGenerator::Generate(world, parameterSet);
}
