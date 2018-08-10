#include "Engine.hpp"
#include "Screen.hpp"
#include "BufferManager.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "RenderManager.hpp"
#include "FrameBuffer.hpp"
#include "Render/ShadowFrameBuffer.hpp"

#include "Game/RenderBuilder.hpp"
#include "Game/Types.hpp"

const Size RenderBuilder::SHADOW_MAP_SIZE = Size(6144, 6144);

const float RenderBuilder::SHADOW_MAP_SIZE_MODIFIER = 1.3f;

RenderBuilder* RenderBuilder::instance_ = new RenderBuilder();

void RenderBuilder::Initialize()
{
	auto screen = Engine::GetScreen();

	FrameBuffer** frameBuffer = BufferManager::GetFrameBuffers().Add(FrameBuffers::DEFAULT);
	if(frameBuffer)
	{
		*frameBuffer = new FrameBuffer(screen->GetSize());

		RenderManager::SetDefaultFrameBuffer(*frameBuffer);
	}

	frameBuffer = BufferManager::GetFrameBuffers().Add(FrameBuffers::SHADOW_MAP);
	if(frameBuffer)
	{
		*frameBuffer = new ShadowFrameBuffer();
		(*frameBuffer)->Initialize(SHADOW_MAP_SIZE, FrameBufferAttachments::DEPTH);
	}

	auto screenTexture = new Texture(screen->GetSize(), TextureFormats::FOUR_BYTE);
	TextureManager::AddTexture(screenTexture, "Screen");

	auto backgroundColor = Color(0.0f, 0.2f, 0.7f, 1.0f);
	RenderManager::SetBackgroundColor(backgroundColor);
}

RenderBuilder::RenderBuilder()
{
	RenderManager::OnInitialize()->Add(&RenderBuilder::Initialize);
}
