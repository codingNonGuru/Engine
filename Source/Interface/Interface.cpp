#include "Interface.hpp"

#include "Element.hpp"
#include "InputHandler.hpp"

#define MAXIMUM_ELEMENT_COUNT 256

Map <Element*, LongWord> Interface::elements_ = Map <Element*, LongWord> (MAXIMUM_ELEMENT_COUNT);

Element* Interface::hoveredElement_ = nullptr;

bool Interface::isSorted_ = false;

Array <Element*> sortedElements = Array <Element*> (MAXIMUM_ELEMENT_COUNT);

void Interface::Initialize() {}

void Interface::Update()
{
	Element* firstElement = nullptr;
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		if(!element->CheckHover())
			continue;

		if(!firstElement)
		{
			firstElement = element;
		}
		else
		{
			if(element->GetDrawOrder() > firstElement->GetDrawOrder())
			{
				firstElement = element;
			}
		}
	}

	hoveredElement_ = firstElement;

	if(firstElement && InputHandler::GetMouse().CurrentLeft_)
	{
		firstElement->HandleClick();
	}

	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		element->Update();
	}
}

void Interface::Render(Camera* camera)
{
	if(!isSorted_)
	{
		Sort();
	}

	for(auto elementIterator = sortedElements.GetStart(); elementIterator != sortedElements.GetEnd(); ++elementIterator)
	{
		auto element = *elementIterator;

		element->Render(camera);
	}
}

void Interface::Sort()
{
	for(auto elementIterator = elements_.GetStart(); elementIterator != elements_.GetEnd(); ++elementIterator)
	{
		*sortedElements.Allocate() = *elementIterator;
	}

	for(int i = 0; i < sortedElements.GetSize(); ++i)
	{
		for(auto elementIterator = sortedElements.GetStart(); elementIterator != sortedElements.GetEnd(); ++elementIterator)
		{
			auto element = *elementIterator;

			if(elementIterator == sortedElements.GetEnd() - 1)
				continue;

			auto nextElement = *(elementIterator + 1);

			if(element->GetDrawOrder() < nextElement->GetDrawOrder())
				continue;

			*elementIterator = *(elementIterator + 1);
			*(elementIterator + 1) = element;
		}
	}

	isSorted_ = true;
}

Map <Element*, LongWord> & Interface::GetElements()
{
	return elements_;
}

Element* Interface::AddElement(const char* name, Element* element)
{
	auto elementPointer = elements_.Add(name);
	if(!elementPointer)
		return nullptr;

	*elementPointer = element;

	isSorted_ = false;

	return element;
}

Element* Interface::GetHoveredElement()
{
	return hoveredElement_;
}
