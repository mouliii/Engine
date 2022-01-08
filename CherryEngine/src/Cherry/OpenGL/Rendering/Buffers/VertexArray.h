#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "GL/glew.h"


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void add_vertex_buffer(const VertexBuffer& vb);
	void bind() const;
	void unbind() const;

private:
	uint32_t id;
};

