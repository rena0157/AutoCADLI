// application.cpp
// By: Adam Renaud
// Created: 2018-05-30

// Purpose: house the functions and class methods for the application class

// Include
#include "include/application.h"
#include <fstream>
#include "include/Entities/lwpolyline.h"


autocadli::application::application(char** args):
	cmdl_(args)
{
	// read the file into lines_
	read_file(cmdl_[1].c_str());
	parse_text();
}

void autocadli::application::read_file(const char* path)
{
	std::ifstream file(path);
	for (std::string line; std::getline(file, line);)
		lines_.push_back(line);
}

void autocadli::application::parse_text()
{
	// Main loop variables
	const regex_patterns patterns;
	object_t object = object_t::null; // starting object type
	
	// Structs
	lwpolyline_buffer lw_buf;

	for (size_t linenum{0}; linenum < lines_.size(); ++linenum)
	{
		std::string& line = lines_[linenum]; // The Current line
		std::smatch match_results;

		// Search for type of object
		if (std::regex_search(line, patterns.lwpolyline_id))
			object = object_t::lwpolyline;
		else if (std::regex_search(line, match_results, patterns.line_id))
			object = object_t::line;

		// Main parse switch
		switch (object)
		{
		case object_t::null:
			break;
		case object_t::lwpolyline:
			lw_buf.parser(line);
			break;
		case object_t::line:
			// TODO: Add line parser
			break;
		default:
			break;
		}
	}
}
