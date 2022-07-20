#pragma once

#include <iostream>
#include <array>

#include <GL/glew.h>
#include <memory>

#include "Buffers/Buffers.h"
#include "..\..\Maths\Vecs.h"
#include "Shader.h"
#include "Texture.h"
#include "OrthoCamera.h"

struct QuadVertex
{
	vec2f pos;
	vec4f color;
};

struct RenderData
{
	//orig : 15k
	const uint32_t max_quad_count = 15000;
	const uint32_t max_vertex_count = max_quad_count * 4;
	const uint32_t max_indices_count = max_quad_count * 6;
	static const uint32_t max_texture_slots = 16;

	QuadVertex* vertex_buffer_base = nullptr;
	QuadVertex* vertex_buffer_ptr = nullptr;

	uint32_t quad_index_count = 0;
	uint32_t white_tex_id = 0;
	std::array<uint32_t, max_texture_slots> texture_slots;
	uint32_t tex_slot_index = 1;
};


class Renderer
{
public:
	Renderer();
	~Renderer();


	void clear();
	void display();


	void draw_quad(const vec2f& pos, const vec2f& size, const vec4f& color);
	void draw_quad(const std::vector<vec2f>& points, const vec4f& color);

	void draw_poly(const std::vector<vec2f>& points, const vec4f& color);

	void set_camera(OrthoCamera* ortho_camera);
	void update_camera();

	OrthoCamera* get_camera() const;

	void DrwaQuad(std::vector<vec2f>& points, vec4f& color);

private:
	void begin_batch();
	void flush();
	void end_batch();

	void FlushNewQuads();

private:
	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;

	RenderData data;

	Shader shader;

	vec4f clear_color = { 0.2f, 0.3f, 0.3f, 1.0f };
	
	OrthoCamera* ortho_cam;

	uint32_t vaoInstanced;
	uint32_t vboInstanced;
	uint32_t iboInstanced;
	uint32_t instanceCount = 0u;
	uint32_t instancedStride = 0u;
};

