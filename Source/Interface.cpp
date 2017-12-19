#include "Interface.hpp"

#include "Element.hpp"
#include "InputHandler.hpp"

#define MAXIMUM_ELEMENT_COUNT 256

Map <Element, LongWord> Interface::elements_ = Map <Element, LongWord> (MAXIMUM_ELEMENT_COUNT);

void Interface::Initialize() {}

void Interface::Update()
{
	Element* firstElement = nullptr;
	for(auto element = elements_.GetStart(); element != elements_.GetEnd(); ++element)
	{
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

	if(firstElement && InputHandler::GetMouse().CurrentLeft_)
	{
		firstElement->Click();
	}

	for(auto element = elements_.GetStart(); element != elements_.GetEnd(); ++element)
	{
		element->Update();
	}
}

void Interface::Render(Camera* camera)
{
	for(auto element = elements_.GetStart(); element != elements_.GetEnd(); ++element)
	{
		element->Render(camera);
	}
}

Map <Element, LongWord> & Interface::GetElements()
{
	return elements_;
}
