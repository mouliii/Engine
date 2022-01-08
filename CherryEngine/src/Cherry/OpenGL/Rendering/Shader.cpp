#include "Shader.h"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
	load_shaders(vertex_path, fragment_path);
}

void Shader::load_shaders(const std::string& vertex_path, const std::string& fragment_path)
{
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream vshader_file;
	std::ifstream fshader_file;

	vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{

		vshader_file.open(vertex_path);
		fshader_file.open(fragment_path);

		std::stringstream vshader_stream, fshader_stream;

		vshader_stream << vshader_file.rdbuf();
		fshader_stream << fshader_file.rdbuf();

		vshader_file.close();
		fshader_file.close();

		vertex_code = vshader_stream.str();
		fragment_code = fshader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();

	uint32_t vs;
	uint32_t fs;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &v_shader_code, NULL);
	glCompileShader(vs);
	check_compile_errors(vs, "VERTEX");

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &f_shader_code, NULL);
	glCompileShader(fs);
	check_compile_errors(fs, "FRAGMENT");
	
	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);

	glLinkProgram(id);

	check_compile_errors(id, "PROGRAM");

	glDeleteShader(vs);
	glDeleteShader(fs);

}

void Shader::bind() const
{
	glUseProgram(id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::set_uniform1f(const std::string& name, float value)
{
	glUniform1f(get_uniform_location(name), value);
}

void Shader::set_uniform1i(const std::string& name, int value)
{
	glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(get_uniform_location(name), v0, v1, v2);
}

void Shader::set_uniform3f(const std::string& name, vec3f v3)
{
	glUniform3f(get_uniform_location(name), v3.x, v3.y, v3.z);
}

void Shader::set_uniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform4f(const std::string& name, vec4f v4)
{
	glUniform4f(get_uniform_location(name), v4.x, v4.y, v4.z, v4.w);
}

void Shader::set_uniform3fv(const std::string& name, mat3f mat)
{
	glUniformMatrix3fv(get_uniform_location(name), 1, GL_TRUE, &mat.cells[0][0]);
}

void Shader::set_uniform4fv(const std::string& name, mat4f mat)
{
	glUniformMatrix4fv(get_uniform_location(name), 1, GL_TRUE, &mat.cells[0][0]);
}

uint32_t Shader::get_id() const
{
	return id;
}

uint32_t Shader::get_uniform_location(const std::string& name)
{
	if (uniform_cache.find(name) != uniform_cache.end())
		return uniform_cache[name];
	else
	{
		uint32_t location = glGetUniformLocation(id, name.c_str());
		uniform_cache[name] = location;
		return location;
	}
}

void Shader::check_compile_errors(uint32_t shader, const std::string& type)
{
	int success;
	char info_log[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, info_log);
			std::cout << "ERROR:SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << std::endl << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, info_log);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << std::endl << std::endl;
		}

	}
}
