#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(uint32_t frame_width, uint32_t frame_height)
{
	glGenFramebuffers(1, &framebuffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	resize_framebuffer(frame_width, frame_height);
}


FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &framebuffer_id);
	glDeleteTextures(1, &texture_id);
}

void FrameBuffer::resize_framebuffer(uint32_t frame_width, uint32_t frame_height)
{
	// color buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_width, frame_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	// depth buffer
	glGenRenderbuffers(1, &renderbuffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frame_width, frame_height);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_id);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_RENDERBUFFER, renderbuffer_id);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_RENDERBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

const uint32_t FrameBuffer::get_framebuffer_tex() const
{
	return texture_id;
}

const uint32_t FrameBuffer::get_framebuffer_id() const
{
	return renderbuffer_id;
}
