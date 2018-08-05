#include "Engine.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "State.hpp"
#include "StateManager.hpp"
#include "Camera.hpp"
#include "RenderManager.hpp"

#include "Game/Game.hpp"
#include "Game/InterfaceBuilder.hpp"
#include "Game/SceneBuilder.hpp"
#include "Game/Filter/FilterBuilder.hpp"
#include "Game/Types.hpp"
#include "Game/States/PregameState.hpp"
#include "Game/States/WorldState.hpp"

Game* Game::instance_ = Game::Register();

Delegate Game::OnStartGame_ = Delegate();

void Game::Initialize()
{
	auto screen = Engine::GetScreen();

	Camera* camera = nullptr;

	camera = new Camera(screen, Position3(0.0f, 0.0f, 0.0f), 0.7f, 0.0f, 3.0f);
	RenderManager::AddCamera(Cameras::PREGAME, camera);

	camera = new Camera(screen, Position3(0.0f, 0.0f, 0.0f), 0.5f, 0.0f, 3.0f);
	RenderManager::AddCamera(Cameras::WORLD, camera);

	auto cubeMesh = MeshManager::GetMeshes().Get("Sphere");
	auto genericShader = ShaderManager::GetShader("Generic");

	if(cubeMesh != nullptr)
	{
		auto cubeModel = new Model(cubeMesh, genericShader);
		ModelManager::AddModel(cubeModel, "Building");
	}

	SceneBuilder::GenerateScenes();

	FilterBuilder::GenerateFilters();

	InterfaceBuilder::GenerateInterface();

	StateManager::Register(new State(), States::INTRO);
	StateManager::Register(new PregameState(), States::PREGAME);
	StateManager::Register(new WorldState(), States::WORLD);
}

void Game::StartGame()
{
	OnStartGame_.Invoke();

	StateManager::Enter(States::WORLD);
}

Game* Game::Register()
{
	Engine::OnInitialize_.Add(&Game::Initialize);

	Engine::OnGameLoopStart_.Add(&Game::HandleGameLoopStart);

	return new Game();
}

void Game::HandleGameLoopStart()
{
	StateManager::Enter(States::PREGAME);
}
