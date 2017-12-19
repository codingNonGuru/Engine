#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Shader;

class Texture
{
	Index key_;

	Size size_;

	TextureFormats format_;

public:
	Texture();

	Texture(Size, TextureFormats);

	~Texture();

	void Bind();

	void Bind(GLuint, Shader*, const char*);

	void Unbind();

	void Upload(container::Matrix<>*, GLenum, GLenum, GLenum);

	void Delete();

	Index GetKey() const {return key_;}

	Size GetSize() const {return size_;}
};


