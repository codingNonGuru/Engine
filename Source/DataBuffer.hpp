#pragma once

#include "Container/Container.hpp"

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class DataBuffer
{
	GLuint key_;

	GLenum type_;

	unsigned int size_;

public:
	DataBuffer() {}

	DataBuffer(int, void* = nullptr);

	GLenum GetType() const {return type_;}

	void Generate(int, void*);

	void UploadData(void*, GLuint);

	void Download(container::Container *);

	void Bind();

	void Bind(GLuint);

	void Unbind();

	void SetSize(unsigned int size) {size_ = size;}

	unsigned int GetSize() const {return size_;}

	void Delete();
};
