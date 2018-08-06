#pragma once

#include "Conventions.hpp"
#include "Types.hpp"
#include "Utility/Color.hpp"

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

    FrameBuffer(Size, FrameBufferAttachments, bool, bool = false);

    void Initialize(Size, FrameBufferAttachments, bool, bool = false);

    void BindBuffer();

    void BindBuffer(GLenum, GLenum);

    void UnbindBuffer(GLenum);

    void BindTexture(Shader*, const char*);

    void UnbindTexture();

    Texture* GetColorTexture();

    Texture* GetDepthTexture();

    void Clear(Color);
};
