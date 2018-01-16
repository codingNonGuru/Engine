#pragma once

#include "Conventions.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Texture;
class TextureBinding;
class ConstantBinding;
class ShaderFile;

class Shader
{
private:
	GLuint key_;

	LongWord name_;

	GLuint shaderKeys_[3];

	Array <ShaderFile*> *shaderFiles_;

	Map <TextureBinding, LongWord> textureBindings_;

	Map <ConstantBinding, LongWord> constantBindings_;

	void Compile(int, GLenum);

	void Link();

	void Parse();

public:
	Shader() {}

	Shader(const char*, Array <ShaderFile*> &);

	void Initialize(const char*, Array <ShaderFile*> &);

	void Bind();

	void Unbind();

	bool BindTexture(Texture*, const char*);

	void SetConstant(void*, const char*);

	void Update();

	Length GetTextureCount() const;

	GLuint GetTextureLocation(const char*);

	Array <ShaderFile*> *GetShaderFiles() {return shaderFiles_;}

	LongWord& GetName() {return name_;}

	void DispatchCompute(Size);

	~Shader(void);
};

class ShaderMap
{
private:
	Map <Shader> shaders_;

public:
	ShaderMap() {}

	ShaderMap(int);

	void Initialize(int);

	bool Add(const char*, const char*, const char*, const char*);

	void Use(const char*);

	void Unuse(const char*);

	GLuint GetTextureLocation(const char*, const char*);

	Shader& Get(const char*);

	Shader& operator [](const char*);
};


