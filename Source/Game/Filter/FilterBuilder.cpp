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

#include "Game/Filter/BlurFilter.hpp"
#include "Game/Filter/SaturateFilter.hpp"

void FilterBuilder::GenerateFilters()
{
	auto saturateEffectShader = ShaderManager::GetShaderMap().Get("SaturateEffect");
	auto screenTexture = TextureManager::GetTextures().Get("Screen");

	auto filter = FilterManager::AddFilter("Saturate", new SaturateFilter());
	filter->Initialize(saturateEffectShader, screenTexture);

	auto blurEffectShader = ShaderManager::GetShaderMap().Get("Blur");

	filter = FilterManager::AddFilter("Blur", new BlurFilter());
	filter->Initialize(blurEffectShader, screenTexture);
}
