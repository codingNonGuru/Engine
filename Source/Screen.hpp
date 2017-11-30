#pragma once

class Screen
{
	int widthInteger_, heightInteger_;
	float widthFloating_, heightFloating_;

public:
	int getWidthInteger() const
	{
		return widthInteger_;
	}

	int getHeightInteger() const
	{
		return heightInteger_;
	}

	float getWidthFloating() const
	{
		return widthFloating_;
	}

	float getHeightFloating() const
	{
		return heightFloating_;
	}

	void Initialize(int width, int height)
	{
		widthInteger_ = width;
		heightInteger_ = height;
		widthFloating_ = (float)width;
		heightFloating_ = (float)height;
	}
};
