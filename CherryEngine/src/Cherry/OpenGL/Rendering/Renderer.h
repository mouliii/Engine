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
#include "Buffers/QuadBufferInstanced.h"

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
	void draw_quad(std::vector<vec2f>& points, vec4f& color);

	uint32_t get_frame_buffer_tex() const;


	void begin_batch();
	void flush();
	void end_batch();

private:
	QuadBuffer display_buffer;
	FrameBuffer frame_buffer;
	Shader shader;
	Shader display_shader;

	vec4f clear_color = { 0.2f, 0.3f, 0.3f, 1.0f };
	uint32_t white_pixel_texture = 0u;
	
	OrthoCamera* ortho_cam;

	QuadBuffer quad_buffer;
	QuadBufferInstanced quad_buffer_instanced;
};

