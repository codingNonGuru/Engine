#include "StencilManager.hpp"

#define STENCIL_CAPACITY 128

Map <Stencil*, StencilManager::Identifier> StencilManager::stencils_ = Map <Stencil*, Identifier> (STENCIL_CAPACITY);

void StencilManager::Add(Stencil* stencil, Word name, Index index)
{
	auto stencilPointer = stencils_.Add(Identifier(name, index));
	if(stencilPointer == nullptr)
	{
		std::cout<<"Stencil Manager cannot allocate new Stencil.\n";
		return;
	}

	*stencilPointer = stencil;
}

Stencil* StencilManager::Get(Word name, Index index)
{
	return *stencils_.Get(Identifier(name, index));
}
