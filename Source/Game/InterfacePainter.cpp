#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>

#include "InterfacePainter.hpp"

#include "Interface/Element.hpp"
#include "Texture.hpp"
#include "TextureManager.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"
#include "Interface/Sprite.hpp"
#include "Interface/Interface.hpp"
#include "Utility/Perlin.hpp"
#include "DataBuffer.hpp"
#include "Utility/Kernel.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Palette.hpp"

Map <DataBuffer, Word> InterfacePainter::buffers_ = Map <DataBuffer, Word> (16);

Map <Array <ElementTextureSet>, InterfacePainter::ElementTextureClass> InterfacePainter::textureSets_ = Map <Array <ElementTextureSet>, ElementTextureClass> (16);

Palette basePalette = Palette(16);

Palette lightPalette = Palette(16);

Shader * InterfacePainter::shader_ = nullptr;

Shader * paperGenerationShader = nullptr;

struct KernelBuffer
{
	Kernel* Kernel_;

	DataBuffer* Buffer_;

	KernelBuffer() {}

	KernelBuffer(Kernel* kernel, DataBuffer* buffer) {Kernel_ = kernel; Buffer_ = buffer;}
};

Map <KernelBuffer> kernelBuffers = Map <KernelBuffer> (8);

KernelBuffer* currentKernel = nullptr;

void InterfacePainter::Initialize()
{
	basePalette.Add(Color(1.0f, 0.0f, 0.0f), Range(-0.7f, -0.2f), Range(0.3f, 1.0f), Weight(0.5f));
	basePalette.Add(Color(1.0f, 0.5f, 0.0f), Range(-0.8f, -0.2f), Range(0.0f, 1.0f), Weight(1.0f));
	basePalette.Add(Color(0.0f, 0.5f, 1.0f), Range(-0.7f, -0.2f), Range(0.3f, 1.0f), Weight(0.1f));

	//lightPalette.Add(Color(1.0f, 0.8f, 0.5f), Range(0.5f, 1.0f), Range(0.0f, 1.0f), Weight(1.0f));

	Size canvasSize(2048, 2048);
	int pixelCount = canvasSize.x * canvasSize.y;

	*buffers_.Add("diffuse") = DataBuffer(pixelCount * 16);
	*buffers_.Add("edgeBlur") = DataBuffer(pixelCount * 4);
	*buffers_.Add("edgeBlurSwap") = DataBuffer(pixelCount * 4);

	SetupKernels();

	GenerateStencils();

	paperGenerationShader = ShaderManager::GetShaderMap().Get("GeneratePaper");
}

void InterfacePainter::SetupKernels()
{
	auto kernel = new Kernel(10);
	kernel->Initialize(KernelTypes::GAUSS, 50.0f);
	auto weights = kernel->GetWeights();

	auto buffer = buffers_.Add("tinyKernel");
	if(buffer)
	{
		*buffer = DataBuffer(weights.GetMemorySize(), weights.GetStart());
	}

	*kernelBuffers.Add("tiny") = KernelBuffer(kernel, buffer);

	kernel = new Kernel(20);
	kernel->Initialize(KernelTypes::GAUSS, 200.0f);
	weights = kernel->GetWeights();

	buffer = buffers_.Add("smallKernel");
	if(buffer)
	{
		*buffer = DataBuffer(weights.GetMemorySize(), weights.GetStart());
	}

	*kernelBuffers.Add("small") = KernelBuffer(kernel, buffer);

	kernel = new Kernel(40);
	kernel->Initialize(KernelTypes::GAUSS, 800.0f);
	weights = kernel->GetWeights();

	buffer = buffers_.Add("mediumKernel");
	if(buffer)
	{
		*buffer = DataBuffer(weights.GetMemorySize(), weights.GetStart());
	}

	*kernelBuffers.Add("medium") = KernelBuffer(kernel, buffer);

	kernel = new Kernel(100);
	kernel->Initialize(KernelTypes::GAUSS, 5000.0f);
	weights = kernel->GetWeights();

	buffer = buffers_.Add("largeKernel");
	if(buffer)
	{
		*buffer = DataBuffer(weights.GetMemorySize(), weights.GetStart());
	}

	*kernelBuffers.Add("large") = KernelBuffer(kernel, buffer);
}

DataBuffer* perlinBuffer = nullptr;

void InterfacePainter::GenerateStencils()
{
	Size sourceSize(2048, 2048);

	Perlin::Generate(sourceSize, Range(0.0f, 1.0f), 0.5f, 0.53f, 16.0f);
	perlinBuffer = Perlin::GetResultBuffer();

	container::LayeredGrid<float> stencils(512, 512, 16);

	*buffers_.Add("stencil") = DataBuffer(stencils.GetMemorySize());

	auto shader = ShaderManager::GetShaderMap().Get("GenerateStencil");
	if(shader)
	{
		shader->Bind();
	}

	buffers_.Get("stencil")->Bind(0);
	perlinBuffer->Bind(1);

	Size stencilSize(stencils.GetWidth(), stencils.GetHeight());
	shader->SetConstant(stencilSize, "stencilSize");
	shader->SetConstant(sourceSize, "sourceSize");
	//shader->SetConstant(&glm::uvec2(stencilSize), "stencilSize");
	//shader->SetConstant(&glm::uvec2(sourceSize), "sourceSize");

	unsigned int x = 0, y = 0;
	for(Index index = 0; index < stencils.GetDepth(); ++index)
	{
		glm::uvec2 offset(x * stencils.GetWidth(), y * stencils.GetHeight());
		shader->SetConstant(offset, "offset");
		shader->SetConstant(index, "stencilIndex");

		shader->DispatchCompute(Size(128, 128));

		x++;
		if(x == sourceSize.x / stencils.GetWidth())
		{
			x = 0;
			y++;
		}
	}

	shader->Unbind();
}

void InterfacePainter::GeneratePaper(Size size, ElementShapes shape)
{
	Perlin::Generate(size, Range(0.0f, 1.0f), 0.3f, 0.5f, 4.0f);

	Size computeSize(size.x / 16, size.y / 16);

	SetupPaperGenerator(size);

	Clear(paperGenerationShader, computeSize, shape);

	currentKernel = kernelBuffers.Get("tiny");

	Blur(paperGenerationShader, computeSize);

	ConvertBlurToAlpha(paperGenerationShader, computeSize);
}

void InterfacePainter::GenerateShadow(Size size, Size computeSize)
{
	SetupPaperGenerator(size);

	currentKernel = kernelBuffers.Get("small");

	Blur(paperGenerationShader, computeSize);
}

void InterfacePainter::HighlightEdges(Size size, Size computeSize, const char* kernelName)
{
	Perlin::Generate(size, Range(0.0f, 1.0f), 1.0f, 0.53f, 8.0f);

	SetupPaperGenerator(size);

	currentKernel = kernelBuffers.Get(kernelName);

	Blur(paperGenerationShader, computeSize);

	ConvertBlurToColor(paperGenerationShader, computeSize);
}

void InterfacePainter::SetupPaperGenerator(Size size)
{
	buffers_.Get("diffuse")->Bind(0);
	perlinBuffer->Bind(1);
	buffers_.Get("edgeBlur")->Bind(2);
	buffers_.Get("edgeBlurSwap")->Bind(3);

	paperGenerationShader->Bind();

	paperGenerationShader->SetConstant(size, "size");
}

void InterfacePainter::Clear(Shader* shader, Size computeSize, ElementShapes shape)
{
	SetStage(Stages::CLEAR);

	Float4 baseColor = Color(0.95f, 0.81f, 0.72f, 1.0f);
	shader->SetConstant(baseColor, "baseColor");

	shader->SetConstant((int)shape, "shape");

	shader->DispatchCompute(computeSize);
}

void InterfacePainter::Blur(Shader* shader, Size computeSize)
{
	currentKernel->Buffer_->Bind(4);

	SetStage(Stages::BLUR_HORIZONTALLY);

	int filterSize = currentKernel->Kernel_->GetSide();
	shader->SetConstant(filterSize, "filterSize");

	shader->DispatchCompute(computeSize);

	SetStage(Stages::BLUR_VERTICALLY);

	filterSize = currentKernel->Kernel_->GetSide();
	shader->SetConstant(filterSize, "filterSize");

	shader->DispatchCompute(computeSize);
}

void InterfacePainter::ConvertBlurToAlpha(Shader* shader, Size computeSize)
{
	SetStage(Stages::CONVERT_BLUR_TO_ALPHA);

	shader->DispatchCompute(computeSize);
}

void InterfacePainter::ConvertBlurToColor(Shader* shader, Size computeSize)
{
	SetStage(Stages::CONVERT_BLUR_TO_COLOR);

	shader->DispatchCompute(computeSize);
}

void InterfacePainter::ApplyBrushes(Size size)
{
	Size computeSize(size.x / 4, size.y / 4);

	auto shader = ShaderManager::GetShaderMap().Get("ApplyStencil");

	if(shader)
	{
		shader->Bind();
	}

	buffers_.Get("stencil")->Bind(0);
	buffers_.Get("diffuse")->Bind(3);

	Size stencilSize(512, 512);
	shader->SetConstant(stencilSize, "stencilSize");
	shader->SetConstant(size, "baseSize");

	for(int pass = 0; pass < 100; ++pass)
	{
		float alpha = utility::GetRandom(0.05f, 0.1f);
		shader->SetConstant(alpha, "alpha");

		//glm::vec3 color = palette.Get(utility::GetRandom(0, 1), false, true); //Utility::throwChance(0.5f) ? glm::vec3(0.7f, 0.3f, 0.1f) : glm::vec3(1.0f, 1.0f, 1.0f);
		Size offset(utility::GetRandom(0, size.x) - size.x / 2, utility::GetRandom(0, size.y) - size.y / 2);
		shader->SetConstant(offset, "offset");

		//glm::vec3 color = utility::RollDice(0.5f) ? glm::vec3(1.0f, 0.5f, 0.0f) : glm::vec3(0.95f, 0.81f, 0.72f);
		//Color color = palette.GetColor(0, true, true);
		Color color = basePalette.GetColor();
		shader->SetConstant(color, "color");

		Index stencilIndex = utility::GetRandom(0, 3);
		shader->SetConstant(stencilIndex, "stencilIndex");

		Index mode = 2;
		shader->SetConstant(mode, "mode");

		shader->DispatchCompute(computeSize);
	}

	shader->Unbind();
}

void InterfacePainter::SetStage(Stages stage)
{
	paperGenerationShader->SetConstant((int)stage, "stage");
}

void InterfacePainter::GenerateTextures(ElementShapes shape, Size size, Texture*& baseTexture, Texture*& shadowTexture)
{
	GeneratePaper(size, shape);

	ApplyBrushes(size);

	//HighlightEdges(size, size / 16, "large");

	HighlightEdges(size, size / 16, "medium");

	//HighlightEdges(size, size / 16, "tiny");

	GenerateShadow(size, size / 16);

	Grid <Color> diffuseMap(size.x, size.y);
	buffers_.Get("diffuse")->Download(&diffuseMap);

	baseTexture = new Texture(Size(diffuseMap.GetWidth(), diffuseMap.GetHeight()), TextureFormats::FOUR_FLOAT, &diffuseMap);

	Grid <float> shadowMap(size.x, size.y);
	buffers_.Get("edgeBlurSwap")->Download(&shadowMap);

	shadowMap *= 0.7f;

	shadowTexture = new Texture(Size(shadowMap.GetWidth(), shadowMap.GetHeight()), TextureFormats::ONE_FLOAT, &shadowMap);
}

ElementTextureSet* InterfacePainter::GetTextureSet(ElementShapes shape, ElementSizes size, Index index)
{
	auto textureSetArray = textureSets_.Get(ElementTextureClass(shape, size));
	if(textureSetArray == nullptr)
		return nullptr;

	return textureSetArray->Get(index);
}

void InterfacePainter::PaintInterface()
{
	Initialize();

	Texture* baseTexture = nullptr;
	Texture* shadowTexture = nullptr;

	GenerateTextures(ElementShapes::SQUARE, Size(768, 1024), baseTexture, shadowTexture);
	TextureManager::AddTexture(baseTexture, "MainMenu");
	TextureManager::AddTexture(shadowTexture, "MainMenuShadow");

	GenerateTextures(ElementShapes::SQUARE, Size(768, 1024), baseTexture, shadowTexture);
	TextureManager::AddTexture(baseTexture, "NewGameMenu");
	TextureManager::AddTexture(shadowTexture, "NewGameMenuShadow");

	GenerateTextures(ElementShapes::SQUARE, Size(768, 1024), baseTexture, shadowTexture);
	TextureManager::AddTexture(baseTexture, "NewWorldMenu");
	TextureManager::AddTexture(shadowTexture, "NewWorldMenuShadow");

	GenerateTextures(ElementShapes::SQUARE, Size(768, 768), baseTexture, shadowTexture);
	TextureManager::AddTexture(baseTexture, "WorldPreviewPanel");
	TextureManager::AddTexture(shadowTexture, "WorldPreviewPanelShadow");

	auto textureSetArray = textureSets_.Add(ElementTextureClass(ElementShapes::ROUND, ElementSizes::SMALL));
	textureSetArray->Initialize(4);

	for(int index = 0; index < textureSetArray->GetCapacity(); ++index)
	{
		GenerateTextures(ElementShapes::ROUND, Size(224, 224), baseTexture, shadowTexture);
		*textureSetArray->Allocate() = ElementTextureSet(baseTexture, shadowTexture);
	}

	textureSetArray = textureSets_.Add(ElementTextureClass(ElementShapes::SQUARE, ElementSizes::SMALL));
	textureSetArray->Initialize(8);

	for(int index = 0; index < textureSetArray->GetCapacity(); ++index)
	{
		GenerateTextures(ElementShapes::SQUARE, Size(512, 256), baseTexture, shadowTexture);
		*textureSetArray->Allocate() = ElementTextureSet(baseTexture, shadowTexture);
	}
}
