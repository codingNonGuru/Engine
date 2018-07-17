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

Map <Camera> RenderManager::cameras_ = Map <Camera> (16);

Color RenderManager::backgroundColor_ = Color();

void RenderManager::Initialize()
{
	auto screen = Engine::GetScreen();

	window_ = new Window(screen);

	glewInit();

	glEnable(GL_DEBUG_OUTPUT);

	*cameras_.Add("main") = Camera(screen, Position3(0.0f, 0.0f, 0.0f), 0.7f, 0.0f, 3.0f);
	*cameras_.Add("interface") = Camera(screen);

	auto frameBuffer = BufferManager::GetFrameBuffers().Add("default");
	if(frameBuffer)
	{
		*frameBuffer = new FrameBuffer(screen->GetSize());
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

	SceneManager::Render();

	DisableDepthTesting();

	SetBlendMode();

	auto interfaceCamera = cameras_.Get("interface");

	FilterManager::Update(interfaceCamera);

	Interface::Render(interfaceCamera);

	if(window_ != nullptr)
	{
		window_->Refresh();
	}
}

void RenderManager::UpdateCameras()
{
	for(auto camera = cameras_.GetStart(); camera != cameras_.GetEnd(); ++camera)
	{
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
	return cameras_.Get(identifier);
}

void RenderManager::ClearDefaultBuffer()
{
	auto defaultFrameBuffer = BufferManager::GetFrameBuffer("default");
	if(defaultFrameBuffer == nullptr)
		return;

	defaultFrameBuffer->BindBuffer();
	defaultFrameBuffer->Clear(backgroundColor_);
}
