#pragma once

#include "Conventions.hpp"

class Texture;
class DataBuffer;
class Shader;

class InterfacePainter
{
	enum class Stages
	{
		CLEAR, BLUR_HORIZONTALLY, BLUR_VERTICALLY, CONVERT_BLUR_TO_ALPHA, CONVERT_BLUR_TO_COLOR
	};

	enum class Shapes
	{
		ROUND, SQUARE
	};

	static Map <DataBuffer, LongWord> buffers_;

	static Shader * shader_;

	static void SetupKernels();

	static void SetupPaperGenerator(Size);

	static void Clear(Shader*, Size);

	static void Blur(Shader*, Size);

	static void ConvertBlurToAlpha(Shader*, Size);

	static void ConvertBlurToColor(Shader*, Size);

	static void HighlightEdges(Size, Size, const char*);

	static void ApplyBrushes(Size);

	static void GenerateShadow(Size, Size);

	static void GeneratePaper(Size);

	static Texture* GenerateBaseTexture(Size);

	static Texture* GenerateShadowTexture(Size);

	static void SetStage(Stages);

public:
	static void Initialize();

	static void PaintInterface();

	static void GenerateTextures(Shapes, Size, Texture*&, Texture*&);

	static void GenerateStencils();
};
