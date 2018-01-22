#pragma once

#include "Conventions.hpp"

class Font;

class FontManager
{
private:
	static Map <Font*, LongWord> fonts_;

public:
	static void LoadFonts();

	static Map <Font*, LongWord> & GetFonts();

	static Font * GetFont(const char*);

	static Font * AddFont(Font*, const char*);
};
