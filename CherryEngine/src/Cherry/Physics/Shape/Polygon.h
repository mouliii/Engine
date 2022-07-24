#pragma once
#include <vector>
#include <unordered_map>
#include "../../Maths/Vecs.h"
#include "../../Maths/Matrices.h"

enum class ShapeType
{
	None = -1,
	Polygon,
	Rectangle

};

const std::unordered_map<ShapeType, std::string> shape_conversion_map =

{
	{ShapeType::Polygon,		"Polygon"},
	{ShapeType::Rectangle,		"Rectangle"},

};

inline const std::string& shape_type_to_string(ShapeType c_type)
{
	return shape_conversion_map.find(c_type)->second;
}

inline ShapeType string_to_shape_type(const std::string& name)
{

	for (const auto& it : shape_conversion_map)
	{
		if (it.second == name)
			return it.first;
	}
	return ShapeType::None;
}

inline std::vector<std::string> get_shapes_as_string()
{
	std::vector<std::string> shapes;

	for (auto& it : shape_conversion_map)
	{
		shapes.push_back(it.second);
	}

	return shapes;
}



class Polygon
{
	
public:
	
	Polygon();
	Polygon(std::vector<vec2f> local_points);
	

	void set_local_points(std::vector<vec2f> new_points);
	void get_local_points(std::vector<vec2f>& local_points);

	void translate(vec2f amount);
	void rotate(float amount);
	void scale(float amount);

	void set_translation(vec2f new_translation);
	void set_rotation(float new_rotation);
	void set_scale(float new_scale);

	vec2f get_translation() const;
	float get_rotation() const;
	float get_scale() const;

	vec2f get_position();

	void get_global_points(std::vector<vec2f>& global_points);
	void update_transformation_matrix();
protected:

	
	

protected:

	mat4f transformation_matrix = mat4f::identity();
	
	std::vector<vec2f> local_points;

	vec2f translation = {0.f, 0.f};
	float rotation = 0.f;
	float _scale = 1.f;

	vec2f origin_offset = { 0.f, 0.f };

};