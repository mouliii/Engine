#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "../../../Maths/Vecs.h"


struct Vertex
{
	struct
	{
		vec2f pos;
	}pos;
	struct
	{
		vec4f color;
	}color;
};