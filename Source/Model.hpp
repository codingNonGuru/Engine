#pragma once

#include "Conventions.hpp"

class Mesh;
class MasterBuffer;
class Camera;
class Texture;
class Shader;

class Model
{
private:
	Mesh* mesh_;

	MasterBuffer* buffer_;

	Shader* shader_;

	Map <Texture*, LongWord> textures_;

	void SetupBuffer();

public:
	void Initialize(Mesh*, Shader*, Length);

	void AddTexture(Texture*, const char*);

	void Render(Camera*);
};
