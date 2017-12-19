#pragma once

#include "Memory.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class DataBuffer
{
	GLuint key_;

	GLenum type_;

	unsigned int size_;

public:
	DataBuffer() {}

	DataBuffer(GLenum, int, void*);

	GLenum GetType() const {return type_;}

	void Generate(GLenum, int, void*);

	void UploadData(void*, GLuint);

	void Bind();

	void Bind(GLuint);

	void Unbind();

	void SetSize(unsigned int size) {size_ = size;}

	unsigned int GetSize() const {return size_;}
};
