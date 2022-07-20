#include "Polygon.h"

Polygon::Polygon()
{
	local_points = { {-1.f, 0.f}, {0.f, -1.f}, {0.f, 1.f}, {1.f, 0.f} };
	update_transformation_matrix();
}

Polygon::Polygon(std::vector<vec2f> local_points)
	:
	local_points(local_points)
{

	update_transformation_matrix();
}

void Polygon::set_local_points(std::vector<vec2f> new_points)
{
	local_points = new_points;
}

void Polygon::get_local_points(std::vector<vec2f>& local_points)
{
	local_points = this->local_points;
}

void Polygon::translate(vec2f amount)
{
	translation += amount;
	update_transformation_matrix();
}

void Polygon::rotate(float amount)
{
	rotation += amount;
	update_transformation_matrix();
}

void Polygon::scale(float amount)
{
	_scale += amount;
	update_transformation_matrix();
}

void Polygon::set_translation(vec2f new_translation)
{
	translation = new_translation;
	update_transformation_matrix();
}

void Polygon::set_rotation(float new_rotation)
{
	rotation = new_rotation;
	update_transformation_matrix();
}

void Polygon::set_scale(float new_scale)
{
	_scale = new_scale;
	update_transformation_matrix();
}

vec2f Polygon::get_translation() const
{
	return translation;
}

float Polygon::get_rotation() const
{
	return rotation;
}

float Polygon::get_scale() const
{
	return _scale;
}

vec2f Polygon::get_position()
{
	return transformation_matrix * local_points[0];
}

void Polygon::update_transformation_matrix()
{
	transformation_matrix = mat4f::identity();
	
	transformation_matrix *= mat4f::translate(translation) * mat4f::rotate_z(rotation) * mat4f::scale(_scale);

	global_points.clear();
	for (size_t i = 0; i < local_points.size(); i++)
	{
		global_points.push_back(transformation_matrix * local_points[i]);

	}
	//transformation_matrix *= mat4f::translate(-origin_offset);

}

std::vector<vec2f>& Polygon::get_global_points()
{
	return global_points;
}
