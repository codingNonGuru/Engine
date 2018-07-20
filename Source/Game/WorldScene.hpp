#pragma once

#include "Scene.hpp"

class World;
struct WorldParameterSet;
class File;
class TerrainModel;

class WorldScene : public Scene
{
	static WorldScene* instance_;

	World* world_;

	TerrainModel* worldModel_;

	void HandleStartGame();

public:
	WorldScene();

	void Initialize(File*);

	void Initialize(const WorldParameterSet&);

	void Update() override;

	void Render() override;

	static World* GetWorld();
};
