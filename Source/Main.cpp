#include "AssetManager.hpp"
#include "Engine.hpp"
#include "Delegate.hpp"

int main()
{
	AssetManager::Initialize();

	Engine::Initialize();

	AssetManager::LoadAssets();

	return 0;
}
