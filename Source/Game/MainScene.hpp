#pragma once

#include "Scene.hpp"

class World;
struct WorldParameterSet;
class File;

class MainScene : public Scene
{
	World* world_;

public:
	void Initialize(File*);

	void Initialize(const WorldParameterSet&);
};
