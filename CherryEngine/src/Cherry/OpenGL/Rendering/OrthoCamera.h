#pragma once
#include "..\..\Maths\Matrices.h"

class OrthoCamera
{
public:

	OrthoCamera(float left, float right, float bottom, float top);

	const mat4f& get_view_projection_matrix() const { return view_projection_matrix; }
	void recalculate_viewproj_matrix();

	const vec2f& get_pos() const;
	const float& get_rotation() const;

	void set_position(vec2f pos);
	void set_rotation(float r);
	
	void resize(float left, float right, float bottom, float top);

private:
	mat4f view_matrix;
	mat4f projection_matrix;
	mat4f view_projection_matrix;
	vec2f position = { 0.f, 0.f };
	vec2f size;
	float rotation = 0.f;
};

