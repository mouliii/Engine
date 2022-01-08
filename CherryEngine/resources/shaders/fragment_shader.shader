#version 330 core

out vec4 frag_color;
in vec4 v_color;

void main()
{
	//fragColor = texture(uTextures[int(vTexIndex)], vTexCoord) * vColor;

	frag_color = v_color;

	//fragColor = vColor;
	// * mix(texture(uTexture, texCoord), texture(uTexture2, texCoord), uAlpha);
}