#include <iostream>

#include "AssetManager.hpp"
#include "Engine.hpp"
#include "Delegate.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"
#include "MeshManager.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Game/InterfaceBuilder.hpp"

int main()
{
	AssetManager::Initialize();

	Engine::Initialize();

	AssetManager::LoadAssets();

	ModelManager::Initialize();

	Model* cubeModel = nullptr;
	auto cubeMesh = MeshManager::GetMeshes().Get(LongWord("Sphere"));
	auto genericShader = ShaderManager::GetShaderMap().Get(LongWord("Generic"));

	if(cubeMesh != nullptr)
	{
		cubeModel = new Model(cubeMesh, genericShader);
		ModelManager::AddModel(cubeModel, "Cube");
	}

	InterfaceBuilder::GenerateInterface();

	Engine::StartGameLoop();

	return 0;
}
