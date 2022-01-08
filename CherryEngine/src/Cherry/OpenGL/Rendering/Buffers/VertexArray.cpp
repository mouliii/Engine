#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArray::add_vertex_buffer(const VertexBuffer& vb)
{
	this->bind();
	vb.bind();

	const std::vector<Attribute>& attributes = vb.get_layout().get_attributes();
	for (size_t i = 0; i < attributes.size(); i++)
	{
		glVertexAttribPointer(i, attributes[i].count, get_opengl_datatype(attributes[i].type), attributes[i].normalized ? GL_TRUE : GL_FALSE,
			vb.get_layout().get_stride(), (const void*)attributes[i].offset);

		glEnableVertexAttribArray(i);
	}

}

void VertexArray::bind() const
{
	glBindVertexArray(id);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

