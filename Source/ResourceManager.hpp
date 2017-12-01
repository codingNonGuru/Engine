#pragma once

class ResourceManager
{
private:
	static void LoadShaders();

	static void LoadTextures();

	static void LoadMeshes();

public:
	static void LoadResources();
};
