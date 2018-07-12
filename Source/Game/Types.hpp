#pragma once

enum class WorldSizeOptions {TINY, SMALL, MEDIUM, LARGE, IMMENSE, COUNT};

enum class ElementShapes
{
	SQUARE, ROUND
};

enum class ElementSizes
{
	SMALL, MEDIUM, LARGE
};

enum class ReliefTypes
{
	OCEAN, LAND
};

enum class WorldPreviewModes
{
	RELIEF, BIOME, POLITY, COUNT
};

enum class DisplaceTerrainBuffers
{
	INDEX_LINKS, POSITION_INPUT, POSITION_OUTPUT, NORMAL_OUTPUT, TANGENT_OUTPUT, COUNT
};
