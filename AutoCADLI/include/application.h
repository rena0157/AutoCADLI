#pragma once
#include "Argh/argh.h"
#include <vector>
#include <regex>

namespace autocadli
{
	class application
	{
	public:
		explicit application(char** args);

		const std::vector<std::string>& get_text() const { return lines_; }

	private:
		argh::parser cmdl_;
		std::vector<std::string> lines_;

		/**
		 * \brief Function that reads the text file given by path into
		 * the vector lines_
		 * \param path Path to the text file
		 */
		void read_file(const char* path);

		void parse_text();
	};


	enum class object_t
	{
		lwpolyline,
		line,
		null
	};

	struct regex_patterns
	{
		std::regex lwpolyline_id{ "LWPOLYLINE" };
		std::regex line_id{ "LINE" };
	};

}