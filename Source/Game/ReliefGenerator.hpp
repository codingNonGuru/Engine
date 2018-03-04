#pragma once

class World;
struct WorldParameterSet;

class ReliefGenerator
{
	static void SetupBuffers(World&, const WorldParameterSet&);

public:
	static void Generate(World&, const WorldParameterSet&);
};
