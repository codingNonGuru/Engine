#include "ResourceManager.hpp"
#include "FileManager.hpp"

void ResourceManager::LoadResources()
{
	FileManager::Initialize();

	FileManager::GetFilesInFolder(".", true);
}
