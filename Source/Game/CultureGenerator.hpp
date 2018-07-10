#pragma once

class World;

class CultureGenerator
{
	static void GenerateSettlements(World&);

	static void GenerateRoads(World&);

	static void GenerateDomains(World&);

public:
	static void Generate(World&);
};
