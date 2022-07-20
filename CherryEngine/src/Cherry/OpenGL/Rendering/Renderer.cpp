#include "Renderer.h"

Renderer::Renderer()
{

	vao.bind();
	vbo.generate_buffer();
	vbo.set_data(nullptr, data.max_vertex_count * sizeof(QuadVertex));

	BufferLayout layout
	{
		{DataType::FLOAT, 2, "a_pos"},
		{DataType::FLOAT, 4, "a_color"}
	};

	vbo.set_layout(layout);
	vao.add_vertex_buffer(vbo);

	data.vertex_buffer_base = new QuadVertex[data.max_vertex_count];

	uint32_t* quad_indices = new uint32_t[data.max_indices_count];

	uint32_t offset = 0;

	for (size_t i = 0; i < data.max_indices_count; i+=6)
	{

		quad_indices[i + 0] = offset + 0; 
		quad_indices[i + 1] = offset + 1;
		quad_indices[i + 2] = offset + 2;

		quad_indices[i + 3] = offset + 3;
		quad_indices[i + 4] = offset + 2;
		quad_indices[i + 5] = offset + 0;

		offset += 4;
	}

	ibo.set_data(quad_indices, data.max_indices_count);
	delete[] quad_indices;

	shader.load_shaders("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
	shader.bind();


	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at: " << __FILE__ << " " << __LINE__ << "\n" << err << std::endl;
	}

	begin_batch();
	vao.unbind();
	vbo.unbind();
	ibo.unbind();

	//////////////////////////////////////////////

	const int maxQuads = 100;
	int indexBufferData[maxQuads * 6];
	int instancedOffset = 0;
	for (size_t i = 0; i < maxQuads; i += 6)
	{

		indexBufferData[i + 0] = instancedOffset + 0;
		indexBufferData[i + 1] = instancedOffset + 1;
		indexBufferData[i + 2] = instancedOffset + 2;
								 
		indexBufferData[i + 3] = instancedOffset + 0;
		indexBufferData[i + 4] = instancedOffset + 2;
		indexBufferData[i + 5] = instancedOffset + 3;
		instancedOffset += 4;
	}

	instancedStride = sizeof(float) * 6;


	glGenVertexArrays(1, &vaoInstanced);
	glGenBuffers(1, &vboInstanced);
	glGenBuffers(1, &iboInstanced);

	glBindVertexArray(vaoInstanced);
	glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
	glBufferData(GL_ARRAY_BUFFER, instancedStride * 4 * maxQuads, nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboInstanced);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferData), indexBufferData, GL_DYNAMIC_DRAW);
		
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, instancedStride,  0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, instancedStride, (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

/*
	float vertices[] = {
		// pos				// color
	-50.0f, -50.0f,       1.0f, 1.0f, 1.0f, 1.0f,
	 50.0f, -50.0f,       1.0f, 1.0f, 1.0f, 1.0f,
	 50.0f,  50.0f,       1.0f, 1.0f, 1.0f, 1.0f,
	-50.0f,  50.0f,       1.0f, 1.0f, 1.0f, 1.0f
	};
	int indices[] = { 0, 1, 2, 0, 2, 3};

	glGenVertexArrays(1, &vaoInstanced);
	glGenBuffers(1, &vboInstanced);
	glGenBuffers(1, &iboInstanced);

	glBindVertexArray(vaoInstanced);
	glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboInstanced);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

*/
}

Renderer::~Renderer()
{ 
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
	shader.unbind();
}

void Renderer::clear()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::begin_batch()
{
	data.quad_index_count = 0;
	data.vertex_buffer_ptr = data.vertex_buffer_base;
}

void Renderer::flush()
{
	
	glDrawElements(GL_TRIANGLES, data.quad_index_count, GL_UNSIGNED_INT, nullptr);

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at: " << __FILE__ << " " << __LINE__ << "\n" << err << std::endl;
	}

}

void Renderer::end_batch()
{
	uint32_t data_size = (uint8_t*)data.vertex_buffer_ptr - (uint8_t*)data.vertex_buffer_base;
	vbo.set_sub_data(data.vertex_buffer_base, data_size);
	flush();
}

void Renderer::display()
{
	clear();
	end_batch();
	FlushNewQuads();
	begin_batch();
}

void Renderer::draw_quad(const vec2f& pos, const vec2f& size, const vec4f& color)
{

	if (data.quad_index_count >= data.max_indices_count)
	{
		end_batch();
		flush();
		begin_batch();
	}

	data.vertex_buffer_ptr->pos = { pos.x, pos.y };
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.vertex_buffer_ptr->pos = { pos.x + size.x, pos.y };
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.vertex_buffer_ptr->pos = { pos.x + size.x, pos.y + size.y };
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.vertex_buffer_ptr->pos = { pos.x, pos.y + size.y };
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.quad_index_count += 6;
}

void Renderer::draw_quad(const std::vector<vec2f>& points, const vec4f& color)
{

	if (data.quad_index_count >= data.max_indices_count)
	{
		end_batch();
		flush();
		begin_batch();
	}



	data.vertex_buffer_ptr->pos = points[0];
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.vertex_buffer_ptr->pos = points[3];
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;
	
	data.vertex_buffer_ptr->pos = points[2];
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.vertex_buffer_ptr->pos = points[1];
	data.vertex_buffer_ptr->color = color;
	data.vertex_buffer_ptr++;

	data.quad_index_count += 6;
}

void Renderer::draw_poly(const std::vector<vec2f>& points, const vec4f& color)
{
	if (data.quad_index_count >= data.max_indices_count)
	{
		end_batch();
		flush();
		begin_batch();
	}

	for (size_t i = 0; i < points.size(); i++)
	{
		data.vertex_buffer_ptr->pos = points[i];
		data.vertex_buffer_ptr->color = color;
		data.vertex_buffer_ptr++;		
	}

	data.quad_index_count += 6;
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

void Renderer::DrwaQuad(std::vector<vec2f>& points, vec4f& color)
{
	std::vector<float> data;
	data.push_back(points[0].x);
	data.push_back(points[0].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[3].x);
	data.push_back(points[3].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[2].x);
	data.push_back(points[2].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);
	data.push_back(points[1].x);
	data.push_back(points[1].y);
	data.push_back(color.x);
	data.push_back(color.y);
	data.push_back(color.z);
	data.push_back(color.w);// neliö   elementit     floatteja
	const float vertexDataSize = 4.0f * 6.0f * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, vboInstanced);
	glBufferSubData(GL_ARRAY_BUFFER,instanceCount * vertexDataSize, vertexDataSize, &data[0]);
	instanceCount++;
}

void Renderer::FlushNewQuads()
{
	shader.bind();
	update_camera();
	glBindVertexArray(vaoInstanced);
	glDrawElements(GL_TRIANGLES, sizeof(float)* 6* instanceCount, GL_UNSIGNED_INT, 0); // offset 0
	instanceCount = 0u;
}