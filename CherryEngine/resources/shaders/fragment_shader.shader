#version 330 core

out vec4 frag_color;
in vec4 v_color;
in vec2 v_uv;

uniform sampler2D u_Texture;

void main()
{
	//fragColor = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;

	//frag_color = v_color;

	frag_color = v_color * texture(u_Texture, v_uv);
}