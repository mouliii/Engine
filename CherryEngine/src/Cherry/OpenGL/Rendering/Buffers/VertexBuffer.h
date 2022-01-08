#pragma once

#include <iostream>


#include "GL/glew.h"
#include "BufferLayout.h"


class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size);
	VertexBuffer();
	~VertexBuffer();
	void generate_buffer();
	
	void set_data(const void* data, uint32_t size);
	void set_sub_data(const void* data, uint32_t size);

	void set_layout(const BufferLayout& layout);
	const BufferLayout& get_layout() const;

	void bind() const;
	void unbind() const;

private:
	uint32_t id;
	BufferLayout layout;
};

