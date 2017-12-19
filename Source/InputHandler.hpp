#pragma once

#include "Conventions.hpp"

class Engine;

struct Mouse
{
	Position2 Position_;

	bool CurrentRight_, CurrentLeft_;

	bool ScrollUp_, ScrollDown_;

	bool LastRight_, LastLeft_;

	Mouse() : Position_(Position2(0.0f, 0.0f)), CurrentRight_(false), CurrentLeft_(false), LastRight_(false), LastLeft_(false), ScrollUp_(false), ScrollDown_(false) {}
};

class InputHandler
{
	static Mouse mouse_;

	static int keyCount_;

	static container::Array<int> currentKeys_;

	static container::Array<int> formerKeys_;

	static void UpdateMouse();

public:
	static void Update();

	static bool IsPressed(int32_t);

	static bool WasPressed(int32_t);

	static void Initialize();

	static Position2 GetMousePosition();

	static const Mouse & GetMouse();
};
