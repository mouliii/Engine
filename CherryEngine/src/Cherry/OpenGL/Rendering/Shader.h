#pragma once

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>


#include "GL/glew.h"

#include "../../Maths/Vecs.h"
#include "../../Maths/Matrices.h"
class Shader
{
public:
	
	Shader() = default;
	Shader(const std::string& vertex_path, const std::string& fragment_path);

	void load_shaders(const std::string& vertex_path, const std::string& fragment_path);
	void bind() const;
	void unbind() const;

	void set_uniform1f(const std::string& name, float value);
	void set_uniform1i(const std::string& name, int value);
	void set_uniform3f(const std::string& name, float v0, float v1, float v2);
	void set_uniform3f(const std::string& name, vec3f v3);
	void set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void set_uniform4f(const std::string& name, vec4f v4);
	void set_uniform3fv(const std::string& name, mat3f mat);
	void set_uniform4fv(const std::string& name, mat4f mat);
	uint32_t get_id() const;
	uint32_t get_uniform_location(const std::string& name);

	void check_compile_errors(uint32_t shader, const std::string& type);


private:
	uint32_t id;
	std::unordered_map<std::string, uint32_t> uniform_cache;
};

