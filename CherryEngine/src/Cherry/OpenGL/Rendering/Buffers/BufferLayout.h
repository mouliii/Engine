#pragma once

#include <vector>

#include "BufferData.h"


class BufferLayout
{

public:
	BufferLayout()=default;
	BufferLayout(std::initializer_list<Attribute> attribute_list);

	void add_attribute(Attribute attribute);
	void calculate_stride_and_offset();

	const uint32_t get_stride() const;
	const uint32_t get_attribute_count() const;

	const std::vector<Attribute>& get_attributes() const;

private:
	std::vector<Attribute> elements;
	uint32_t stride = 0;
	uint32_t offset = 0;
};

