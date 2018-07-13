#include "Engine.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include "Game/Game.hpp"
#include "Game/InterfaceBuilder.hpp"
#include "Game/SceneBuilder.hpp"
#include "Game/Filter/FilterBuilder.hpp"

Game* Game::instance_ = Game::Register();

void Game::Initialize()
{
	Model* cubeModel = nullptr;
	auto cubeMesh = MeshManager::GetMeshes().Get("Sphere");
	auto genericShader = ShaderManager::GetShaderMap().Get("Generic");

	if(cubeMesh != nullptr)
	{
		cubeModel = new Model(cubeMesh, genericShader);
		ModelManager::AddModel(cubeModel, "Cube");
	}

	SceneBuilder::GenerateScenes();

	FilterBuilder::GenerateFilters();

	InterfaceBuilder::GenerateInterface();
}

Game* Game::Register()
{
	Engine::OnInitialize_.Add(&Game::Initialize);

	return new Game();
}
