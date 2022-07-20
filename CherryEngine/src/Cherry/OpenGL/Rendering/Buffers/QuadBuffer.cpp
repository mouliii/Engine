#include "QuadBuffer.h"

QuadBuffer::QuadBuffer()
	:
	vbo(nullptr, 4 * 6 * sizeof(float)),
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

void QuadBuffer::set_data(std::vector<vec2f> points, vec4f color)
{
	std::vector<float> data;
	data.push_back(points[0].x);
	data.push_back(points[0].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[3].x);
	data.push_back(points[3].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[2].x);
	data.push_back(points[2].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[1].x);
	data.push_back(points[1].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);// neliö   elementit     floatteja
	const float vertexDataSize = 4.0f * 6.0f * sizeof(float);
	vbo.set_data(&data, data.size() * sizeof(float));
}

void QuadBuffer::bind()
{
	vao.bind();
}

void QuadBuffer::unbind()
{
	vao.unbind();
}
