#include <GL/glew.h>

#include "RenderManager.hpp"

#include "Camera.hpp"
#include "Engine.hpp"
#include "Screen.hpp"
#include "Window.hpp"
#include "ModelManager.hpp"
#include "Model.hpp"
#include "BufferManager.hpp"
#include "FrameBuffer.hpp"
#include "Interface/Sprite.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "TextureManager.hpp"
#include "Texture.hpp"
#include "Render/FilterManager.hpp"
#include "Render/Filter.hpp"
#include "Utility/Color.hpp"
#include "SceneManager.hpp"

Window* RenderManager::window_ = nullptr;

Map <Camera*> RenderManager::cameras_ = Map <Camera*> (16);

Color RenderManager::backgroundColor_ = Color();

const Size RenderManager::SHADOW_MAP_SIZE = Size(6144, 6144);

const float RenderManager::SHADOW_MAP_SIZE_MODIFIER = 1.3f;

Word interfaceCameraKey = Word("Interface");

void RenderManager::Initialize()
{
	auto screen = Engine::GetScreen();

	window_ = new Window(screen);

	auto camera = new Camera(screen);
	AddCamera(interfaceCameraKey, camera);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);

	auto frameBuffer = BufferManager::GetFrameBuffers().Add("default");
	if(frameBuffer)
	{
		*frameBuffer = new FrameBuffer(screen->GetSize());
	}

	frameBuffer = BufferManager::GetFrameBuffers().Add("shadow");
	if(frameBuffer)
	{
		*frameBuffer = new FrameBuffer(SHADOW_MAP_SIZE, FrameBufferAttachments::DEPTH, false, true);
	}

	auto screenTexture = new Texture(screen->GetSize(), TextureFormats::FOUR_BYTE);
	TextureManager::AddTexture(screenTexture, "Screen");

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_SHADING);

	DEBUG_OPENGL

	backgroundColor_ = Color(0.0f, 0.2f, 0.7f, 1.0f);
}

void RenderManager::Update()
{
	UpdateCameras();

	ClearDefaultBuffer();

	SceneManager::Render();

	DisableDepthTesting();

	SetBlendMode();

	FilterManager::Update();

	Interface::Render();

	if(window_ != nullptr)
	{
		window_->Refresh();
	}
}

void RenderManager::UpdateCameras()
{
	for(auto cameraIterator = cameras_.GetStart(); cameraIterator != cameras_.GetEnd(); ++cameraIterator)
	{
		auto camera = *cameraIterator;
		if(camera == nullptr)
			continue;

		camera->Update();
	}
}

void RenderManager::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDepthRange(0.0f, 1.0f);

	DEBUG_OPENGL
}

void RenderManager::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);

	DEBUG_OPENGL
}

void RenderManager::SetBlendMode()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DEBUG_OPENGL
}

void RenderManager::DisableBlending()
{
	glDisable(GL_BLEND);

	DEBUG_OPENGL
}

Camera* RenderManager::GetCamera(Word identifier)
{
	auto cameraPointer = cameras_.Get(identifier);
	if(cameraPointer == nullptr)
		return nullptr;

	return *cameraPointer;
}

void RenderManager::AddCamera(Word identifier, Camera* camera)
{
	auto cameraPointer = cameras_.Add(identifier);
	if(cameraPointer == nullptr)
		return;

	*cameraPointer = camera;
}

Camera* RenderManager::GetInterfaceCamera()
{
	return GetCamera(interfaceCameraKey);
}

void RenderManager::ClearDefaultBuffer()
{
	auto defaultFrameBuffer = BufferManager::GetFrameBuffer("default");
	if(defaultFrameBuffer == nullptr)
		return;

	defaultFrameBuffer->BindBuffer();
	defaultFrameBuffer->Clear(backgroundColor_);
}

void RenderManager::SetBackgroundColor(Color color)
{
	backgroundColor_ = color;
}
