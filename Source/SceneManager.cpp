#include "SceneManager.hpp"

#include "Scene.hpp"

#define MAXIMUM_SCENE_COUNT 32

Map <Scene*, LongWord> SceneManager::scenes_ = Map <Scene*, LongWord> (MAXIMUM_SCENE_COUNT);

Scene * SceneManager::GetScene(const char* sceneName)
{
	auto scenePointer = scenes_.Get(sceneName);
	if(!scenePointer)
		return nullptr;

	return *scenePointer;
}

Scene * SceneManager::AddScene(Scene* scene, const char* sceneName)
{
	auto scenePointer = scenes_.Add(sceneName);
	if(!scenePointer)
		return nullptr;

	*scenePointer = scene;

	return *scenePointer;
}

void SceneManager::Update()
{
	for(auto sceneIterator = scenes_.GetStart(); sceneIterator != scenes_.GetEnd(); ++sceneIterator)
	{
		auto scene = *sceneIterator;
		if(!scene)
			continue;

		if(!scene->IsGloballyActive())
			continue;

		scene->Update();
	}
}

void SceneManager::Render()
{
	for(auto sceneIterator = scenes_.GetStart(); sceneIterator != scenes_.GetEnd(); ++sceneIterator)
	{
		auto scene = *sceneIterator;
		if(!scene)
			continue;

		if(!scene->IsGloballyActive())
			continue;

		scene->Render();
	}
}