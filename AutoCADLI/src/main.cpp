#include "Argh/argh.h"
#include <iostream>
#include <vector>
#include "include/application.h"

int main(int argc, char* argv[])
{
	autocadli::application test(argv);

	for (const auto& line : test.get_text())
		std::cout << line << "\n";





	return 0;
}

