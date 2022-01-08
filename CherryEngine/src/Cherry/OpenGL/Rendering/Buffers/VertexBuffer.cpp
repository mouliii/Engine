#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}

void VertexBuffer::generate_buffer()
{
	glGenBuffers(1, &id);
}


void VertexBuffer::set_data(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::set_sub_data(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::set_layout(const BufferLayout& layout)
{
	this->layout = layout;
}
const BufferLayout& VertexBuffer::get_layout() const
{
	return layout;
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
