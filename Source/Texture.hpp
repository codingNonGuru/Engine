#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Shader;
class File;

class Texture
{
	Index key_;

	Size size_;

	TextureFormats format_;

	File* file_;

	void ProcessInternalFormat();

	void ProcessPngFormat();

public:
	Texture();

	Texture(Size, TextureFormats, container::Matrix* = nullptr);

	Texture(File*);

	void Initialize(File*);

	void Upload(void*);

	void Bind();

	void Bind(GLuint, Shader*, const char*);

	void Unbind();

	void DownloadBackBuffer();

	void Delete();

	Index GetKey() const {return key_;}

	Size GetSize() const {return size_;}

	~Texture();
};


