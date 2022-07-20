#include "Renderer.h"

Renderer::Renderer()
{
	shader.load_shaders("resources/shaders/vertex_shader.shader", "resources/shaders/fragment_shader.shader");
	shader.bind();

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

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at: " << __FILE__ << " " << __LINE__ << "\n" << err << std::endl;
	}

}

Renderer::~Renderer()
{ 
	shader.unbind();
}

void Renderer::clear()
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::begin_batch()
{
	instanceCount = 0u;
}

void Renderer::flush()
{
	shader.bind();
	update_camera();
	glBindVertexArray(vaoInstanced);
	glDrawElements(GL_TRIANGLES, 6 * instanceCount, GL_UNSIGNED_INT, 0); // offset 0
}

void Renderer::end_batch()
{
	flush();
}

void Renderer::display()
{
	clear();
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