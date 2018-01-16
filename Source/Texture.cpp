#include <stdlib.h>
#include "GL/glew.h"

#include "Texture.hpp"
#include "Shader.hpp"
#include "File.hpp"

Texture::Texture()
{

}

Texture::Texture(Size size, TextureFormats format, container::Matrix* grid = nullptr)
{
	size_ = size;

	format_ = format;

	void* data = grid ? grid->GetData() : nullptr;
	Upload(data);
}

Texture::Texture(File* file)
{
	Initialize(file);
}

void Texture::Initialize(File* file)
{
	FILE* fileStream = fopen(file->GetPath(), "rb");
	if(fileStream == nullptr)
		return;

	fread(&size_, sizeof(size_), 1, fileStream);
	fread(&format_, sizeof(format_), 1, fileStream);

	container::Matrix * grid = nullptr;

	switch(format_)
	{
	case TextureFormats::FOUR_BYTE:
		grid = new Grid <Byte4> (size_.x, size_.y);
		break;
	case TextureFormats::FOUR_FLOAT:
		grid = new Grid <Float4> (size_.x, size_.y);
		break;
	case TextureFormats::THREE_BYTE:
		grid = new Grid <Byte3> (size_.x, size_.y);
		break;
	case TextureFormats::THREE_FLOAT:
		grid = new Grid <Float3> (size_.x, size_.y);
		break;
	case TextureFormats::ONE_BYTE:
		grid = new Grid <Byte> (size_.x, size_.y);
		break;
	case TextureFormats::ONE_FLOAT:
		grid = new Grid <Float> (size_.x, size_.y);
		break;
	}

	fread(grid->GetData(), grid->GetMemorySize(), 1, fileStream);

	fclose(fileStream);

	Upload(grid->GetData());
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

void Texture::Upload(void* data)
{
	glGenTextures(1, &key_);
	glBindTexture(GL_TEXTURE_2D, key_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch(format_)
	{
	case TextureFormats::FOUR_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_.x, size_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;
	case TextureFormats::FOUR_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size_.x, size_.y, 0, GL_RGBA, GL_FLOAT, data);
		break;
	case TextureFormats::THREE_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size_.x, size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	case TextureFormats::THREE_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size_.x, size_.y, 0, GL_RGB, GL_FLOAT, data);
		break;
	case TextureFormats::ONE_BYTE:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size_.x, size_.y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		break;
	case TextureFormats::ONE_FLOAT:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size_.x, size_.y, 0, GL_RED, GL_FLOAT, data);
		break;
	}


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);

	DEBUG_OPENGL
}

void Texture::DownloadBackBuffer()
{
	Bind();
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glReadBuffer(GL_BACK);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, size_.x, size_.y, 0);
	Unbind();
}

void Texture::Delete()
{
	glBindTexture(GL_TEXTURE_2D, key_);
	glDeleteTextures(1, &key_);

	DEBUG_OPENGL
}

