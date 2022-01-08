#version 330 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 view_proj;


void main() 
{

	gl_Position = view_proj * vec4(a_pos.xy, 0.f, 1.f);//vec4(a_pos.xy, 0.f, 1.f);
	v_color = a_color;
}