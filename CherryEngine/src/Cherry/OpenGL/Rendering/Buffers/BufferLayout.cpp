#include "BufferLayout.h"

BufferLayout::BufferLayout(std::initializer_list<Attribute> attribute_list)
	:
	elements(attribute_list)
{
	calculate_stride_and_offset();
}

void BufferLayout::add_attribute(Attribute attribute)
{
	elements.push_back(attribute);
	calculate_stride_and_offset();
}

void BufferLayout::calculate_stride_and_offset()
{
	stride = 0;
	offset = 0;

	for (size_t i = 0; i < elements.size(); i++)
	{
		elements[i].offset = offset;
		offset += get_datatype_size(elements[i].type) * elements[i].count;
		stride += get_datatype_size(elements[i].type) * elements[i].count;
	}
}

const uint32_t BufferLayout::get_stride() const
{
	return stride;
}

const uint32_t BufferLayout::get_attribute_count() const
{
	return (uint32_t)elements.size();
}

const std::vector<Attribute>& BufferLayout::get_attributes() const
{
	return elements;
}
