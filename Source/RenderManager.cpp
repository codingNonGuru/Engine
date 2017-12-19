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
#include "Sprite.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Interface.hpp"
#include "Element.hpp"

RenderManager* RenderManager::instance_ = nullptr;

RenderManager* RenderManager::Get()
{
	if(instance_ == nullptr)
	{
		instance_ = new RenderManager();
	}

	return instance_;
}

void RenderManager::Initialize()
{
	screen_ = Engine::GetScreen();

	cameras_.Initialize(16);

	*cameras_.Allocate(LongWord("main")) = Camera(screen_, Position3(0.0f, 0.0f, 0.0f), 0.7f, 0.0f, 3.0f);
	*cameras_.Allocate(LongWord("interface")) = Camera(screen_);

	auto frameBuffer = BufferManager::GetFrameBuffers().Allocate(LongWord("default"));
	if(frameBuffer)
	{
		*frameBuffer = new FrameBuffer(Size(screen_->getWidthInteger(), screen_->getHeightInteger()));
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_SHADING);

	DEBUG_OPENGL
}

void RenderManager::Update()
{
	UpdateCameras();

	auto mainCamera = cameras_.Get("main");
	auto interfaceCamera = cameras_.Get("interface");

	EnableDepthTesting();

	auto defaultFrameBuffer = BufferManager::GetFrameBuffer("default");
	if(defaultFrameBuffer)
	{
		defaultFrameBuffer->BindBuffer();
		defaultFrameBuffer->Clear(Color(0.0f, 0.3f, 0.5f, 1.0f));
	}

	auto cubeModel = ModelManager::GetModels().Get(LongWord("Cube"));
	cubeModel->Render(mainCamera);

	DisableDepthTesting();

	SetBlendMode();

	Interface::Render(interfaceCamera);

	auto window = Engine::GetWindow();
	if(window != nullptr)
	{
		window->Refresh();
	}
}

void RenderManager::UpdateCameras()
{
	for(auto camera = cameras_.GetStart(); camera != cameras_.GetEnd(); ++camera)
	{
		camera->Spin(0.0003f);
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
