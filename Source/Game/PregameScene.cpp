#include "Model.hpp"
#include "ModelManager.hpp"
#include "RenderManager.hpp"

#include "Game/PregameScene.hpp"

PregameScene::PregameScene()
{
	camera_ = RenderManager::GetCamera("main");
}

void PregameScene::Render()
{
	auto cubeModel = ModelManager::GetModels().Get("Cube");
	cubeModel->Render(camera_);
}
