#include "Renderer.h"

Renderer::Renderer()
	:
	frame_buffer(800, 600),
	quad_buffer_instanced(1000, GL_DYNAMIC_DRAW)
{
	shader.load_shaders("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
	shader.bind();
}

Renderer::~Renderer()
{ 
	shader.unbind();
}

void Renderer::clear()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::begin_batch()
{
	quad_buffer_instanced.reset_instance_count();
}

void Renderer::flush()
{
	shader.bind();
	update_camera();
	quad_buffer_instanced.bind();
	glDrawElements(GL_TRIANGLES, 6 * quad_buffer_instanced.get_instance_count(), GL_UNSIGNED_INT, 0); // offset 0
	// tarviiko vvv
	quad_buffer_instanced.unbind();
	shader.unbind();
}

void Renderer::end_batch()
{
	flush();
}

void Renderer::display()
{
	//clear(); clear framen alussa
	end_batch();
	begin_batch();
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
	std::vector<float> data;
	data.reserve(24);

	for (size_t i = 0; i < 4; i++)
	{
		data.push_back(points[i].x);
		data.push_back(points[i].y);
		data.push_back(color.x);
		data.push_back(color.y);
		data.push_back(color.z);
		data.push_back(color.w); 
	}
	quad_buffer_instanced.set_data(points, color);
}

void Renderer::draw_quad(std::vector<vec2f>& points, vec4f& color)
{
	quad_buffer.bind();
	quad_buffer.set_data(points, color);
	shader.bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // offset 0
	glBindVertexArray(0);
	shader.unbind();
}
