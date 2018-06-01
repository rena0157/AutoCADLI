#include "Argh/argh.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	const char* path = R"(C:\Dev\AutoCADLI\TestFiles\test.txt)";
	std::ifstream file;

	file.open(path, std::ios::ate);

	const auto file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	auto* file_contents = new char;
	file.read(file_contents, file_size);
	file.close();
	std::cout << file_contents;



	delete file_contents;
	file_contents = nullptr;


	return 0;
}

