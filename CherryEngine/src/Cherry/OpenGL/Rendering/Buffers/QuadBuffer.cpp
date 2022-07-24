#include "QuadBuffer.h"

QuadBuffer::QuadBuffer()
	:
	vbo(nullptr, 4 * 8 * sizeof(float), GL_DYNAMIC_DRAW),
	ibo(nullptr, 6)
{
	int arr[] = { 0,1,2,0,2,3 };
	ibo.set_data(arr, 6);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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
	data[0].uv = vec2f(0, 0);
	data[1].uv = vec2f(1, 0);
	data[2].uv = vec2f(1, 1);
	data[3].uv = vec2f(0, 1);

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
