#include <iostream>

#include <GL/glew.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "DataBuffer.hpp"

#include "Conventions.hpp"

DataBuffer::DataBuffer(int memorySize, void* data)
{
	Generate(memorySize, data);
}

void DataBuffer::Generate(int memorySize, void* data)
{
	type_ = GL_SHADER_STORAGE_BUFFER;

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

void DataBuffer::Download(Container * container)
{
	Bind();

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, container->GetMemorySize(), container->GetData());

	Unbind();
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

void DataBuffer::Delete()
{
	Bind();
	glDeleteBuffers(1, &key_);
	Unbind();
}


