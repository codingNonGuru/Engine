#pragma once

#include "Conventions.hpp"

class Element;
class Camera;

class Interface
{
private:
	static Map <Element, LongWord> elements_;

public:
	static void Initialize();

	static void Update();

	static void Render(Camera*);

	static Map <Element, LongWord> & GetElements();
};
