#pragma once

#include <iostream>


#include "GL/glew.h"
#include "BufferLayout.h"


class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size, GLenum draw_type);
	~VertexBuffer();
	void set_data(uint32_t offset, uint32_t size, const void* data);
	void bind() const;
	void unbind() const;

private:
	uint32_t id;
	GLenum usage;
};

