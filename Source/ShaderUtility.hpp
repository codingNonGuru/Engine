#pragma once

enum class ShaderConstantTypes
{
	INTEGER_1, INTEGER_2, INTEGER_3, INTEGER_4,
	FLOAT_1, FLOAT_2, FLOAT_3, FLOAT_4,
	UINTEGER_1, UINTEGER_2, UINTEGER_3, UINTEGER_4,
	MATRIX_3, MATRIX_4
};

class ShaderFile
{
private:
	const char* path_;

public:
	ShaderFile() {}

	ShaderFile(const char* path) {path_ = path;}

	const char* GetPath() {return path_;}

	void SetPath(const char* path) {path_ = path;}
};

class TextureBinding
{
private:
	Index unitIndex_;

	Index location_;

public:
	TextureBinding() {}

	TextureBinding(Index unitIndex, Index location) : unitIndex_(unitIndex), location_(location) {}

	Index GetUnitIndex() const {return unitIndex_;}

	Index GetLocation() const {return location_;}
};

class ConstantBinding
{
private:
	Index location_;

	ShaderConstantTypes type_;

public:
	ConstantBinding() {}

	ConstantBinding(Index location, ShaderConstantTypes type) : location_(location), type_(type) {}

	Index GetLocation() const {return location_;}

	ShaderConstantTypes GetType() const {return type_;}
};
