#pragma once

#include "Conventions.hpp"

class Mesh;
class DataBuffer;
class Camera;
class Texture;
class Shader;

enum class Shaders;

class Model
{
protected:
	Mesh* mesh_;

	Map <DataBuffer*, LongWord> buffers_;

	Map <Shader*, Shaders> shaders_;

	Map <Texture*, LongWord> textures_;

	void SetupBuffer();

public:
	Model();

	Model(Mesh*, Shader*);

	virtual void Initialize();

	void Initialize(Mesh*, Shader*);

	void AddTexture(Texture*, const char*);

	virtual void Render(Camera*);
};
