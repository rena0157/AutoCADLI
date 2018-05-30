#include "Argh/argh.h"
#include <iostream>


int main(int argc, char* argv[])
{
	argh::parser cmdl(argv);

	if (cmdl[{"-v", "--verbose"}])
		std::cout << "Hello There I am Verbose!\n";

	return 0;
}

