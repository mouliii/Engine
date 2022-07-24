#version 330 core

out vec4 frag_color;
in vec4 v_color;
in vec2 v_uv;

uniform sampler2D u_Texture;

void main()
{
	vec3 albedo = vec3(1.0f) -  texture(u_Texture, v_uv).rgb;
	frag_color = vec4(albedo,1.0);
}