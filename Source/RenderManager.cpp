#include <GL/glew.h>

#include "RenderManager.hpp"

#include "Camera.hpp"
#include "Engine.hpp"
#include "Screen.hpp"
#include "Window.hpp"
#include "ModelManager.hpp"
#include "BufferManager.hpp"
#include "FrameBuffer.hpp"
#include "ShaderManager.hpp"
#include "Interface/Interface.hpp"
#include "Interface/Element.hpp"
#include "TextureManager.hpp"
#include "Texture.hpp"
#include "Render/FilterManager.hpp"
#include "Utility/Color.hpp"
#include "SceneManager.hpp"

Window* RenderManager::window_ = nullptr;

Map <Camera*> RenderManager::cameras_ = Map <Camera*> (16);

Color RenderManager::backgroundColor_ = Color();

FrameBuffer* RenderManager::defaultFrameBuffer_ = nullptr;

Delegate* RenderManager::onInitialize_ = nullptr;

Word interfaceCameraKey = Word("Interface");

void RenderManager::Initialize()
{
	auto screen = Engine::GetScreen();

	window_ = new Window(screen);

	auto camera = new Camera(screen);
	AddCamera(interfaceCameraKey, camera);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_SHADING);

	if(onInitialize_ != nullptr)
	{
		onInitialize_->Invoke();
	}
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
	if(defaultFrameBuffer_ == nullptr)
		return;

	defaultFrameBuffer_->BindBuffer();
	defaultFrameBuffer_->Clear(backgroundColor_);
}

void RenderManager::SetBackgroundColor(Color color)
{
	backgroundColor_ = color;
}

Color RenderManager::GetBackgroundColor()
{
	return backgroundColor_;
}

void RenderManager::SetDefaultFrameBuffer(FrameBuffer* defaultFrameBuffer)
{
	defaultFrameBuffer_ = defaultFrameBuffer;
}

Delegate* RenderManager::OnInitialize()
{
	if(onInitialize_ == nullptr)
		onInitialize_ = new Delegate();

	return onInitialize_;
}
