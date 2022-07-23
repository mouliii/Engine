#include "QuadBufferInstanced.h"

QuadBufferInstanced::QuadBufferInstanced(uint32_t instance_count, GLenum draw_type)
	:
	maxInstances(instance_count),
	vbo(nullptr, instance_count * sizeof(Vertex) * 4, draw_type),
	ibo(nullptr, instance_count * 6)
{
	int indexCount = instance_count * 6;
	uint32_t* indexBufferData = new uint32_t[indexCount];
	int indexOffset = 0;
	for (size_t i = 0; i < indexCount; i += 6)
	{
		indexBufferData[i + 0] = indexOffset + 0;
		indexBufferData[i + 1] = indexOffset + 1;
		indexBufferData[i + 2] = indexOffset + 2;

		indexBufferData[i + 3] = indexOffset + 0;
		indexBufferData[i + 4] = indexOffset + 2;
		indexBufferData[i + 5] = indexOffset + 3;
		indexOffset += 4;
	}
	vao.bind();
	vbo.bind();
	vbo.set_data(0, sizeof(Vertex) * 4 * instance_count, nullptr);

	ibo.set_data(indexBufferData, indexCount);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	delete[] indexBufferData;
}

QuadBufferInstanced::~QuadBufferInstanced()
{
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
}

void QuadBufferInstanced::set_data(std::vector<vec2f>& points, vec4f& color)
{
	Vertex data[4];
	for (size_t i = 0; i < 4; i++)
	{
		data[i].pos = { points[i] };
		data[i].color = { color };
	}
	const float vertexDataSize = sizeof(Vertex) * 4;
	vbo.set_data(instanceCount * vertexDataSize, vertexDataSize, &data[0]);
	instanceCount++;
}

void QuadBufferInstanced::draw()
{
	// hmm
}

void QuadBufferInstanced::bind()
{
	vao.bind();
}

void QuadBufferInstanced::unbind()
{
	vao.unbind();
}

void QuadBufferInstanced::reset_instance_count()
{
	instanceCount = 0u;
}

const uint32_t QuadBufferInstanced::get_instance_count() const
{
	return instanceCount;
}

const uint32_t QuadBufferInstanced::get_max_instance_count() const
{
	return maxInstances;
}
