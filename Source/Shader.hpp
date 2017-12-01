#pragma once

#include "Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
class TextureBinding;
class ConstantBinding;
class ShaderFile;

enum class ShaderTypes
{
	RENDER,
	COMPUTE
};

class Shader
{
private:
	GLuint key_;

	GLuint shaderKeys_[3];

	Array <ShaderFile> shaderFiles_;

	Map <TextureBinding, LongWord> textureBindings_;

	Map <ConstantBinding, LongWord> constantBindings_;

	void Compile(int, GLenum);

	void Link();

	void Parse();

public:
	void Initialize(const char*);

	void Initialize(const char*, const char*);

	void Initialize(const char*, const char*, const char*);

	Shader() {}

	Shader(const char*);

	Shader(const char*, const char*);

	void Bind();

	void Unbind();

	bool BindTexture(Texture*, const char*);

	void AddUniform(const char*);

	void Update();

	GLuint GetTextureLocation(const char*);

	Array <ShaderFile> &GetShaderFiles() {return shaderFiles_;}

	~Shader(void);
};

class ShaderMap
{
private:
	Map <Shader> shaders_;

public:
	void Initialize(int);

	bool Add(const char*, const char*, const char*, const char*);

	void Use(const char*);

	void Unuse(const char*);

	GLuint GetTextureLocation(const char*, const char*);

	Shader& Get(const char*);

	Shader& operator [](const char*);
};


