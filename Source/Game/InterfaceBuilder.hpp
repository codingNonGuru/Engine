#pragma once

class Element;

class InterfaceBuilder
{
	static void GenerateMainMenu();

	static void GenerateMainMenuNewGameButton();

	static void GenerateNewGameMenu();

	static void GenerateNewGameMenuBackButton();

	static void GenerateNewWorldButton();

	static void GenerateNewWorldMenu();

	static void GenerateNewWorldMenuSizeButton();

	static void GenerateNewWorldMenuLeftScrollButton();

	static void GenerateNewWorldMenuRightScrollButton();

	static void GenerateNewWorldMenuGenerateButton();

	static void GenerateNewWorldMenuStartGameButton();

	static void GenerateNewWorldMenuBackButton();

	static void GenerateWorldPreview();

	static void GenerateWorldPreviewViewModeButtons();

	static void AddOpenAnimation(Element*, float, float);

	static void AddCloseAnimation(Element*, float, float);

public:
	static void GenerateInterface();
};
