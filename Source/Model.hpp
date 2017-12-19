#pragma once

#include "Conventions.hpp"

class Mesh;
class HeaderBuffer;
class Camera;
class Texture;
class Shader;

class Model
{
private:
	Mesh* mesh_;

	HeaderBuffer* buffer_;

	Shader* shader_;

	Map <Texture*, LongWord> textures_;

	void SetupBuffer();

public:
	Model();

	Model(Mesh*, Shader*);

	void Initialize(Mesh*, Shader*);

	void AddTexture(Texture*, const char*);

	void Render(Camera*);
};
