#pragma once

class Game
{
private:
	static Game* Register();

	static Game* instance_;

public:
	static void Initialize();

	static void HandleGameLoopStart();
};
