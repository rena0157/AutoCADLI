#include "include/Entities/lwpolyline.h"

int autocadli::lwpolyline_buffer::parser(std::string&)
{
	// TODO: Implement this function
	return 0;
}

autocadli::lwpolyline::lwpolyline(lwpolyline_buffer& pl_buf) : 
	entity_base(pl_buf), length_(pl_buf.length)
{

}
