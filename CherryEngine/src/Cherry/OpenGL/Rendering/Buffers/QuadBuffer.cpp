#include "QuadBuffer.h"

QuadBuffer::QuadBuffer()
	:
	vbo(nullptr, 4 * 6 * sizeof(float), GL_DYNAMIC_DRAW),
	ibo(nullptr, 6)
{
	int arr[] = { 0,1,2,0,2,3 };
	ibo.set_data(arr, 6);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

QuadBuffer::~QuadBuffer()
{
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
}

void QuadBuffer::set_data(std::vector<vec2f>& points, vec4f& color)
{
	Vertex data[4];
	for (size_t i = 0; i < 4; i++)
	{
		data[i].pos = { points[i] };
		data[i].color = { color };
	}
	const float vertexDataSize = sizeof(Vertex) * 4;
	vbo.set_data(0, vertexDataSize, &data[0]);
}

void QuadBuffer::bind()
{
	vao.bind();
}

void QuadBuffer::unbind()
{
	vao.unbind();
}
