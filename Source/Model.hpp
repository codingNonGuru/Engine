#pragma once

class Mesh;
class MasterBuffer;
class Camera;

class Model
{
private:
	Mesh* mesh_;

	MasterBuffer* buffer_;

public:
	void Initialize(Mesh*);

	void Render(Camera*);
};
