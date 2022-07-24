#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "../../../Maths/Vecs.h"


struct Vertex
{
	vec2f pos;
	vec4f color;
	vec2f uv;
};