#include "Text.hpp"

#include "Interface/TextManager.hpp"
#include "Interface/Glyph.hpp"
#include "Interface/Font.hpp"
#include "DataBuffer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

Text::Text() {}

Text::Text(Font* font, Color color = Color::BLACK)
{
	font_ = font;

	color_ = color;
}

void Text::Setup(const char* string)
{
	string_ = string;
}

void Text::Assemble()
{
	auto& dataQueue = TextManager::GetDataQueue();

	dataQueue.Reset();

	auto textPosition = GetGlobalPosition();

	auto startPosition = textPosition;

	float textWidth = 0.0f;

	for(auto sign = string_.Get(); sign != string_.GetEnd(); ++sign)
	{
		auto glyph = font_->GetGlyph(*sign);

		if(!glyph)
			continue;

		auto glyphData = dataQueue.Allocate();

		if(!glyphData)
			continue;

		*glyphData = glyph->GetData();

		glyphData->Color_ = color_;

		startPosition.x += glyph->GetAdvance() * 0.5f;

		glyphData->Position_ = startPosition;

		startPosition.x += glyph->GetAdvance() * 0.5f;

		textWidth += glyph->GetAdvance();
	}

	for(auto glyphData = dataQueue.GetStart(); glyphData != dataQueue.GetEnd(); ++glyphData)
	{
		glyphData->Position_.x -= textWidth * 0.5f;
	}
}

void Text::Render(Camera* camera)
{
	if(!IsGloballyActive())
		return;

	Assemble();

	auto& dataQueue = TextManager::GetDataQueue();

	auto buffer = TextManager::GetBuffer();

	buffer->UploadData(dataQueue.GetStart(), dataQueue.GetMemorySize());

	auto shader = TextManager::GetShader();

	auto texture = font_->GetTexture();

	shader->Bind();

	buffer->Bind(0);

	shader->SetConstant(camera->GetMatrix(), "viewMatrix");

	Opacity opacity = 1.0f;
	shader->SetConstant(opacity, "opacity");

	auto drawOrder = (float)drawOrder_ * 0.1f;
	shader->SetConstant(drawOrder, "depth");

	if(texture && shader)
	{
		shader->BindTexture(texture, "diffuse");
	}

	glDrawArrays(GL_TRIANGLES, 0, dataQueue.GetSize() * 6);

	shader->Unbind();

	if(texture)
	{
		texture->Unbind();
	}
}
