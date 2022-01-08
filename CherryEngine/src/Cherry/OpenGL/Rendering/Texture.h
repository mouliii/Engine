#pragma once


#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& filepath, GLenum type = GL_TEXTURE_2D);
	~Texture();

	void bind() const;
	void unbind() const;
	void delete_tex() const;
	const uint32_t& get_id() const;

private:
	uint32_t width;
	uint32_t height;
	uint32_t n_channels;
	uint32_t id = 0;

	GLenum type;


};

