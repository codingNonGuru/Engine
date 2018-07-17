#pragma once

#include "Types.hpp"
#include "Conventions.hpp"

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

enum class States
{
	NONE, INTRO, PREGAME, WORLD, BATTLE
};

enum class Scenes
{
	WORLD, BATTLE
};

class TerrainModelBuffers
{
public:
	static LongWord const INDEX_LINKS;
	static LongWord const POSITION_INPUT;
	static LongWord const POSITION_OUTPUT;
	static LongWord const NORMAL_OUTPUT;
	static LongWord const TANGENT_OUTPUT;
	static LongWord const INDICES;
	static int const COUNT;
};

class Elements
{
public:
	static LongWord const MAIN_MENU;
	static LongWord const NEW_GAME_MENU;
	static LongWord const NEW_WORLD_MENU;
	static LongWord const WORLD_PREVIEW_PANEL;
};
