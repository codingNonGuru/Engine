#pragma once

class World;
struct WorldParameterSet;

class WorldGenerator
{
public:
	static void Generate(World&, const WorldParameterSet&);
};
