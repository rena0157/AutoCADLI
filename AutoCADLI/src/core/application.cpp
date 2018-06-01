// application.cpp
// By: Adam Renaud
// Created: 2018-05-30

// Purpose: house the functions and class methods for the application class

// Include
#include "include/application.h"
#include <fstream>


autocadli::application::application(char** args):
	cmdl_(args)
{
	// Start command loop

}

void autocadli::application::read_file(const char* path)
{

	std::ifstream file(path);

	while (file.good() && file.is_open())
	{

	}

}
