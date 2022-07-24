#version 330 core

out vec4 frag_color;
in vec4 v_color;
in vec2 v_uv;

uniform sampler2D u_Texture;

void main()
{
	frag_color = texture(u_Texture, v_uv);
}