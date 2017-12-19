#include "MeshManager.hpp"

#include "AssetManager.hpp"
#include "File.hpp"
#include "Mesh.hpp"

Map <Mesh, LongWord> MeshManager::meshes_ = Map <Mesh, LongWord>();

#define MAXIMUM_MESH_COUNT 256

void MeshManager::Initialize()
{
}

void MeshManager::LoadMeshes()
{
	meshes_.Initialize(MAXIMUM_MESH_COUNT);

	auto files = AssetManager::GetFiles();

	for(File* file = files->GetStart(); file != files->GetEnd(); ++file)
	{
		auto meshExtension = ".mesh";

		auto extensionPosition = FindString(file->GetName(), meshExtension);
		if(extensionPosition == nullptr)
			continue;

		LongWord meshName;
		meshName.Add(file->GetName(), extensionPosition - file->GetName());

		auto mesh = meshes_.Allocate(meshName);
		if(mesh == nullptr)
			continue;

		mesh->Initialize(file);
	}
}

Map <Mesh, LongWord> & MeshManager::GetMeshes()
{
	return meshes_;
}
