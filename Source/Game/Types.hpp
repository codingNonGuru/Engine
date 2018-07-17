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
	PREGAME, WORLD, BATTLE
};

class TerrainModelBuffers
{
public:
	static Word const INDEX_LINKS;
	static Word const POSITION_INPUT;
	static Word const POSITION_OUTPUT;
	static Word const NORMAL_OUTPUT;
	static Word const TANGENT_OUTPUT;
	static Word const INDICES;
	static int const COUNT;
};

class Elements
{
public:
	static Word const MAIN_MENU;
	static Word const NEW_GAME_MENU;
	static Word const NEW_WORLD_MENU;
	static Word const WORLD_PREVIEW_PANEL;
};

class Cameras
{
public:
	static Word const PREGAME;
};
