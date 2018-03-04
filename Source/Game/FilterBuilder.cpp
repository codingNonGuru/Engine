#include "FilterBuilder.hpp"

#include "ShaderManager.hpp"
#include "Shader.hpp"
#include "Animation.hpp"
#include "Animator.hpp"
#include "AnimationProperty.hpp"
#include "AnimationKey.hpp"
#include "AnimationEvent.hpp"
#include "TextureManager.hpp"
#include "Texture.hpp"
#include "Render/FilterManager.hpp"
#include "Render/Filter.hpp"

void FilterBuilder::GenerateFilters()
{
	auto saturateEffectShader = ShaderManager::GetShaderMap().Get("SaturateEffect");
	auto screenTexture = TextureManager::GetTextures().Get("Screen");

	auto filter = FilterManager::AddFilter("Saturate", new Filter());
	filter->Initialize(saturateEffectShader, screenTexture);
}
