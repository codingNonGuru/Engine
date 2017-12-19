#include <iostream>

#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "DataBuffer.hpp"

#include "Conventions.hpp"

DataBuffer::DataBuffer(GLenum type, int memorySize, void* data = nullptr)
{
	Generate(type, memorySize, data);
}

void DataBuffer::Generate(GLenum type, int memorySize, void* data = nullptr)
{
	type_ = type;

	glGenBuffers(1, &key_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, memorySize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	DEBUG_OPENGL
}

void DataBuffer::UploadData(void* data, GLuint size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	DEBUG_OPENGL
}

void DataBuffer::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);

	DEBUG_OPENGL
}

void DataBuffer::Bind(GLuint bindPoint)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, key_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindPoint, key_);

	DEBUG_OPENGL
}

void DataBuffer::Unbind()
{
	glBindBuffer(type_, 0);

	DEBUG_OPENGL
}




