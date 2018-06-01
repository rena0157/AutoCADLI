#pragma once
#include "Argh/argh.h"

namespace autocadli
{
	class application
	{
	public:
		explicit application(char** args);

	private:
		argh::parser cmdl_;

		void read_file(const char* path);
	};
}