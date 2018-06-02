#include "include/Entities/entity.h"

int autocadli::entity_buffer::parser(std::string&)
{
	return 0;
}

autocadli::entity_base::entity_base(entity_buffer& eb) :
	handle_(eb.handle), layer_(eb.layer)
{

}
