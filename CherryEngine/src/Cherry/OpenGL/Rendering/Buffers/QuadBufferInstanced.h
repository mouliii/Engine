#pragma once

#include "Buffers.h"
#include <vector>
#include "../../../Maths/Vecs.h"

class QuadBufferInstanced
{
public:
	QuadBufferInstanced(uint32_t instance_count, GLenum draw_type);
	~QuadBufferInstanced();
	void set_data(std::vector<vec2f>& points, vec4f& color);
	void draw();
	void bind();
	void unbind();
	void reset_instance_count();
	const uint32_t get_instance_count() const;
	const uint32_t get_max_instance_count() const;
private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	uint32_t instanceCount = 0u;
	uint32_t maxInstances;
};