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

#include "Buffers/QuadBuffer.h"

class Renderer
{
public:
	Renderer();
	~Renderer();


	void clear();
	void display();

	void set_camera(OrthoCamera* ortho_camera);
	void update_camera();

	OrthoCamera* get_camera() const;

	void draw_quad_instanced(std::vector<vec2f>& points, vec4f& color);

private:
	void begin_batch();
	void flush();
	void end_batch();

private:
	Shader shader;

	vec4f clear_color = { 0.2f, 0.3f, 0.3f, 1.0f };
	
	OrthoCamera* ortho_cam;

	uint32_t vaoInstanced;
	uint32_t vboInstanced;
	uint32_t iboInstanced;
	uint32_t instanceCount = 0u;
	uint32_t instancedStride = 0u;
};

