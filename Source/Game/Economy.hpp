#pragma once

class Settlement;

class Economy
{
	Settlement* settlement_;

	float resource_;

	float development_;

	float infrastructure_;

	float technology_;

public:
	Economy();

	Economy(Settlement*);

	void Update();
};
