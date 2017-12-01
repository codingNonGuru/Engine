#include <iostream>
#include <stdio.h>
#include <dirent.h>

#include "FileManager.hpp"

Array <File> FileManager::files_ = Array <File>();

void FileManager::Initialize()
{
	files_.initialize(4096);
}

Array <File>& FileManager::GetFilesInFolder(const char* path, bool isRecursive)
{
	files_.reset();

	GetFilesInSubfolder(path, isRecursive);

	return files_;
}

void FileManager::GetFilesInSubfolder(const char* path, bool isRecursive)
{
#ifdef LINUX

	DIR *directory;
	struct dirent *file;

	directory = opendir(path);
	if (directory != nullptr)
	{
		while (file = readdir(directory))
		{
			char filePath[256];
			strcpy(filePath, path);
			strcat(filePath, "/");
			strcat(filePath, file->d_name);
			if(file->d_type != DT_DIR)
			{
				std::cout<<"Filename: "<<file->d_name<<"\n";
				std::cout<<"Filepath: "<<filePath<<"\n\n";
				*files_.allocate() = File(file->d_name, filePath);
			}

			if(!isRecursive)
				continue;

			if(file->d_type == DT_DIR && strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0)
			{


				GetFilesInSubfolder(filePath, true);
			}
		}
	}
	closedir(directory);
#endif
}

