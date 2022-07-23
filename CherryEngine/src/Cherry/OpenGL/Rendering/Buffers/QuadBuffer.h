#pragma once

#include "Buffers.h"
#include <vector>
#include "../../../Maths/Vecs.h"

class QuadBuffer
{
public:
	QuadBuffer();
	~QuadBuffer();
	void set_data(std::vector<vec2f>& points, vec4f& color);
	void bind();
	void unbind();
private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
};