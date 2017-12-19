#include <stdlib.h>
#include "GL/glew.h"

#include "Texture.hpp"
#include "Shader.hpp"

Texture::Texture()
{

}

Texture::Texture(Size size, TextureFormats format)
{
	size_ = size;

	format_ = format;

	glGenTextures(1, &key_);
	glBindTexture(GL_TEXTURE_2D, key_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch(format_)
	{
	case TextureFormats::FOUR_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
	case TextureFormats::FOUR_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, 0);
		break;
	case TextureFormats::THREE_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		break;
	case TextureFormats::THREE_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size.x, size.y, 0, GL_RGB, GL_FLOAT, 0);
		break;
	case TextureFormats::ONE_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		break;
	case TextureFormats::ONE_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size.x, size.y, 0, GL_RED, GL_FLOAT, 0);
		break;
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);

	DEBUG_OPENGL
}

Texture::~Texture()
{

}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, key_);

	DEBUG_OPENGL
}

void Texture::Bind(GLuint index, Shader* shader, const char* name)
{
	GLuint location = shader->GetTextureLocation(name);
	glUniform1i(location, index);
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, key_);

	DEBUG_OPENGL
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	DEBUG_OPENGL
}

void Texture::Upload(container::Matrix<> *buffer, GLenum internalFormat, GLenum format, GLenum type)
{
	glGenTextures(1, &key_);
	glBindTexture(GL_TEXTURE_2D, key_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, buffer->getWidth(), buffer->getHeight(), 0, format, type, buffer->GetData());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);

	size_ = Size(buffer->getWidth(), buffer->getHeight());

	DEBUG_OPENGL
}

void Texture::Delete()
{
	glBindTexture(GL_TEXTURE_2D, key_);
	glDeleteTextures(1, &key_);

	DEBUG_OPENGL
}

