#pragma once

class Shader;
class Texture;
class Camera;

class Filter
{
private:
	Shader* shader_;

	Texture* screenTexture_;

public:
	Filter();

	Filter(Shader*, Texture*);

	void Initialize(Shader*, Texture*);

	void Apply(Camera*);
};
