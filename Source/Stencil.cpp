#include "Stencil.hpp"
#include "DataBuffer.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"

Stencil::Stencil()
{}

Stencil::Stencil(Size size, DataBuffer* sourceBuffer, Size sourceSize, Size offset)
{
	size_ = size;

	buffer_ = new DataBuffer(size.x * size.y * 4);

	auto shader = ShaderManager::GetShaderMap().Get("GenerateStencil");
	if(shader)
	{
		shader->Bind();
	}

	buffer_->Bind(0);
	sourceBuffer->Bind(1);

	shader->SetConstant(size, "stencilSize");
	shader->SetConstant(sourceSize, "sourceSize");

	shader->SetConstant(offset, "offset");

	shader->DispatchCompute(size / 4);

	shader->Unbind();
}

void Stencil::Bind(Index index)
{
	buffer_->Bind(index);
}

Size Stencil::GetSize() const
{
	return size_;
}

void Stencil::Apply(DataBuffer* canvasBuffer, Size canvasSize, Float alpha, Color& color, Size offset)
{
	auto shader = ShaderManager::GetShaderMap().Get("ApplyStencil");

	shader->Bind();

	buffer_->Bind(0);
	canvasBuffer->Bind(3);

	shader->SetConstant(canvasSize, "baseSize");
	shader->SetConstant(size_, "stencilSize");
	shader->SetConstant(alpha, "alpha");
	shader->SetConstant(offset, "offset");
	shader->SetConstant((Float4&)color, "color");

	Index mode = 2;
	shader->SetConstant(mode, "mode");

	shader->DispatchCompute(size_ / 4);

	shader->Unbind();
}
