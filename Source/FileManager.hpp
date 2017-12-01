#pragma once

#include "Conventions.hpp"

class File
{
private:
	const char* name_;

	const char* path_;

public:
	File() {}

	File(const char*, const char*) {}

	const char* GetName() {return name_;}

	const char* GetPath() {return path_;}
};

class FileManager
{
private:
	static Array <File> files_;

	static void GetFilesInSubfolder(const char*, bool);

public:
	static void Initialize();

	static Array <File>& GetFilesInFolder(const char*, bool);
};
