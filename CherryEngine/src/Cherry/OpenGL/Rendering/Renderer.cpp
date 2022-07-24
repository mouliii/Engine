#include "Renderer.h"
#include "../../Logic/stb_image.h"

Renderer::Renderer()
	:
	frame_buffer(800, 600),
	quad_buffer_instanced(1000, GL_DYNAMIC_DRAW),
	display_shader("resources/shaders/display_vertex.glsl", "resources/shaders/display_fragment.glsl")
{
	shader.load_shaders("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
	shader.bind();

	//
	glGenTextures(1, &white_pixel_texture);
	glBindTexture(GL_TEXTURE_2D, white_pixel_texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/wooden_container.jpg", &width, &height, &nrChannels, 0);
	if (data)	
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	std::vector<vec2f> p{ vec2f(0.0f,0.0f), vec2f(800.0f,0.0f), vec2f(800.0f,600.0f), vec2f(0.0f,600.0f) };
	vec4f c = { 1.f, 1.f, 1.f, 1.f };
	display_buffer.set_data(p,c);
}

Renderer::~Renderer()
{ 
	shader.unbind();
}

void Renderer::clear()
{
	frame_buffer.bind();
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::begin_batch()
{
	quad_buffer_instanced.reset_instance_count();
}

void Renderer::flush()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, white_pixel_texture);

	shader.bind();
	update_camera(); 
	shader.set_uniform1i("u_Texture", 0); // 0 on texture slot
	quad_buffer_instanced.bind();
	glDrawElements(GL_TRIANGLES, 6 * quad_buffer_instanced.get_instance_count(), GL_UNSIGNED_INT, 0); // offset 0
	// tarviiko vvv
	quad_buffer_instanced.unbind();
	shader.unbind();
}

void Renderer::end_batch()
{
	// draw as usual
	// first pass
	flush();
	frame_buffer.unbind();
	// default frame buffer
	// second pass
	glClearColor(0.3f, 0.0f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frame_buffer.get_framebuffer_tex());

	display_shader.bind();
	display_buffer.bind();

	display_shader.set_uniform1i("u_Texture", 1);
	display_shader.set_uniform4fv("view_proj", ortho_cam->get_view_projection_matrix());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::display()
{
	// ei kytössä atm
	//clear(); clear framen alussa
	//end_batch();
	//begin_batch();
}

void Renderer::set_camera(OrthoCamera* ortho_camera)
{
	ortho_cam = ortho_camera;
	this->shader.set_uniform4fv("view_proj", ortho_cam->get_view_projection_matrix());
}

void Renderer::update_camera()
{
	this->shader.set_uniform4fv("view_proj", ortho_cam->get_view_projection_matrix());
}

OrthoCamera* Renderer::get_camera() const
{
	return ortho_cam;
}

void Renderer::draw_quad_instanced(std::vector<vec2f>& points, vec4f& color)
{
	if (quad_buffer_instanced.get_instance_count() >= quad_buffer_instanced.get_max_instance_count())
	{
		flush();
		quad_buffer_instanced.reset_instance_count();
	}
	quad_buffer_instanced.set_data(points, color);
}

void Renderer::draw_quad(std::vector<vec2f>& points, vec4f& color)
{
	quad_buffer.bind();
	quad_buffer.set_data(points, color);
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, white_pixel_texture);
	shader.set_uniform1i("u_Texture", white_pixel_texture);
	shader.bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // offset 0
	glBindVertexArray(0);
	shader.unbind();
	quad_buffer.unbind();
}

uint32_t Renderer::get_frame_buffer_tex() const
{
	return frame_buffer.get_framebuffer_tex();
}
