#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "GL/glew.h"


class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unbind() const;

private:
	uint32_t id;
};

