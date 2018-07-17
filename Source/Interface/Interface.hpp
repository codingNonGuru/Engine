#pragma once

#include "Conventions.hpp"

class Element;
class Camera;

class Interface
{
private:
	static Map <Element*, LongWord> elements_;

	static Element* hoveredElement_;

	static bool isSorted_;

	static void Sort();

public:
	static void Initialize();

	static void Update();

	static void Render(Camera*);

	static Map <Element*, LongWord> & GetElements();

	static Element* GetElement(LongWord);

	static Element* AddElement(LongWord, Element*);

	static Element* GetHoveredElement();
};
