#pragma once

#include "Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

enum class ShaderTypes
{
	RENDER,
	COMPUTE
};

class TextureBinding
{
private:
	ShortWord name_;

	Index index_;

public:
	TextureBinding() {}

	TextureBinding(const char*, Index);
};

class ShaderFile
{
private:
	const char* path_;

public:
	ShaderFile() {}

	ShaderFile(const char* path) {path_ = path;}

	const char* GetPath() {return path_;}

	void SetPath(const char* path) {path_ = path;}
};

class Shader
{
private:
	GLuint key_;

	GLuint shaderKeys_[3];

	Array <ShaderFile> shaderFiles_;

	Array <TextureBinding> textureBindings_;

	void Compile(int, GLenum);

	void Link();

public:
	void Initialize(const char*);

	void Initialize(const char*, const char*);

	void Initialize(const char*, const char*, const char*);

	Shader() {}

	Shader(const char*);

	Shader(const char*, const char*);

	void Bind();

	void Unbind();

	void AddUniform(const char*);

	void Update();

	GLuint GetTextureLocation(const char*);

	Array <ShaderFile> &GetShaderFiles() {return shaderFiles_;}

	~Shader(void);
};

class ShaderMap
{
private:
	container::StaticMap <Shader> shaders_;

public:
	void Initialize(int);

	bool Add(const char*, const char*, const char*, const char*);

	void Use(const char*);

	void Unuse(const char*);

	GLuint GetTextureLocation(const char*, const char*);

	Shader& Get(const char*);

	Shader& operator [](const char*);
};


