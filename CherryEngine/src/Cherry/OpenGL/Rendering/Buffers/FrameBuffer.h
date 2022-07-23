#pragma once

#include "Buffers.h"
#include "../../../Maths/Vecs.h"


class FrameBuffer
{
public:
	FrameBuffer(uint32_t frame_width, uint32_t frame_height);
	~FrameBuffer();
	void resize_framebuffer(uint32_t frame_width, uint32_t frame_height);
	void bind();
	void unbind();
	const uint32_t get_framebuffer_tex() const;
	const uint32_t get_framebuffer_id() const;
private:
	uint32_t framebuffer_id = 0;
	uint32_t renderbuffer_id = 0u;
	uint32_t texture_id = 0;
};