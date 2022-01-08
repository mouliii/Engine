#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
	:
	count(0u)
{
	glGenBuffers(1, &id);
}

IndexBuffer::IndexBuffer(const void* data, uint32_t count)
	:
	count(count)
{
	glGenBuffers(1, &id);
	this->set_data(data, count);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &id);
}

void IndexBuffer::set_data(const void* data, uint32_t count)
{
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
	this->count = count;
}

const uint32_t& IndexBuffer::get_count() const
{
	return count;
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
