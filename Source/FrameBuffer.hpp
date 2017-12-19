#pragma once

#include "Conventions.hpp"
#include "Types.hpp"

class Shader;
class Texture;

class FrameBuffer
{
private:
	FrameBufferTypes type_;

	Size size_;

	Index bufferKey_, depthBufferKey_;

	Texture* colorTexture_;

	Texture* depthTexture_;

	GLenum textureType_;

public:

	FrameBuffer();

    FrameBuffer(Size);

    void Initialize(Size, FrameBufferAttachments, bool);

    void BindBuffer();

    void BindBuffer(GLenum, GLenum);

    void UnbindBuffer(GLenum);

    void BindTexture(Shader*, const char*);

    void UnbindTexture();

    void Clear(Color);
};
