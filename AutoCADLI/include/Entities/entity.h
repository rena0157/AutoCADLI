#pragma once
#include <string>

namespace autocadli
{

	struct entity_buffer
	{
		// Members
		std::string layer;
		std::string handle;

		// Methods
		virtual ~entity_buffer() = default;
		virtual int parser(std::string&);
	};

	class entity_base
	{
	public:
		explicit entity_base(entity_buffer&);
		virtual ~entity_base() = default;

	private:
		std::string handle_;
		std::string layer_;
	};
}