#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
	:
	projection_matrix(mat4f::ortho(left, right, bottom, top, -1.f, 1.f)),
	view_matrix(mat4f::identity()),
	size(right, top)
{
	view_projection_matrix = projection_matrix * view_matrix;
}

void OrthoCamera::recalculate_viewproj_matrix()
{
	// calculate new transform 
	mat4f transform = mat4f::translate(position) * mat4f::rotate_z(rotation); 

	// inverse transform matrix to get the correct view matrix
	view_matrix = transform.inverse();
	view_projection_matrix = projection_matrix * view_matrix;

}

const vec2f& OrthoCamera::get_pos() const
{
	return position;
}

const float& OrthoCamera::get_rotation() const
{
	return rotation;
}

void OrthoCamera::set_position(vec2f pos)
{
	position = pos;
	recalculate_viewproj_matrix();
}

void OrthoCamera::set_rotation(float r)
{
	rotation = r;
	recalculate_viewproj_matrix();
}

void OrthoCamera::resize(float left, float right, float bottom, float top)
{
	projection_matrix = mat4f::ortho(left, right, bottom, top, -1, 1);
	recalculate_viewproj_matrix();
}
