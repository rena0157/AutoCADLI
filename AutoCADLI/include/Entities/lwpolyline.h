#pragma once
#include "entity.h"


namespace autocadli
{
	struct lwpolyline_buffer : entity_buffer
	{
		// Members
		double length;

		// Methods
		int parser(std::string&) override;
	};

	class lwpolyline : public entity_base
	{
	public:
		explicit lwpolyline(lwpolyline_buffer&);

	private:
		double length_;
	};
}

