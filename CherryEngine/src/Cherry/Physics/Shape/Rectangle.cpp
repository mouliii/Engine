#include "Rectangle.h"

Rectangle::Rectangle()
{
	local_points = { {-1.f, -1.f}, {1.f, -1.f}, {1.f, 1.f}, {-1.f, 1.f} };
	update_transformation_matrix();
}

Rectangle::Rectangle(vec2f position, vec2f size)
{
	local_points.clear();
	local_points.emplace_back(position);
	local_points.emplace_back(position.x + size.x, position.y);
	local_points.emplace_back(position.x + size.x, position.y + size.y);
	local_points.emplace_back(position.x, position.y + size.y);

	origin_offset = size * 0.5f;
	
	
	update_transformation_matrix();
}

vec2f Rectangle::get_size()
{
	
	std::vector<vec2f>& global_points = get_global_points();

	vec2f dimensions;

	dimensions.x = (global_points[1] - global_points[0]).lenght();
	dimensions.y = (global_points[3] - global_points[0]).lenght();
	
	return dimensions;
}
