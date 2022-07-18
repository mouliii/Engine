#pragma once
#include <algorithm>
#include <unordered_map>

#include "../Maths/BitSetLogic.h"
#include "../Physics/Shape/Rectangle.h"

#include "CrEngine.h"

#define COLLISION_MASK_LAYER_COUNT 20



enum class ComponentType
{
	ShapeComponent,
	CollisionComponent,
	DisplayComponent

};

const std::unordered_map<ComponentType, std::string> component_conversion_map =

{
	{ComponentType::ShapeComponent,		"ShapeComponent"},
	{ComponentType::CollisionComponent, "CollisionComponent"},
	{ComponentType::DisplayComponent,	"DisplayComponent"},
};

inline const std::string& component_type_to_string(ComponentType c_type)
{
	return component_conversion_map.find(c_type)->second;
}



class ShapeComponent : public Component
{
public:

	ShapeComponent(Polygon shape);

	Polygon* get_shape();

	void move(vec2f translation);


private:

	Polygon shape;
	
};

class CollisionComponent : public Component
{
public:
	struct CollisionInformation
	{
		vec2f resolution;
	};

public:

	CollisionComponent(ShapeComponent* s_component);

	void on_update_tick(TimeStep dt) override;

	bool AABB(Rectangle& a, Rectangle& b);
	bool move_and_collide(const vec2f& translation);
	bool polygon_intersect(CollisionComponent* other, vec2f& normal, float& depth);

	vec2f find_center(std::vector<vec2f>& vertices);


	void flip_collision_mask_bit(uint32_t bit_index);
	void set_collision_mask_bit(uint32_t bit_index, bool b);
	void flip_all_bits() { collision_mask.flip(); }

	const std::bitset<COLLISION_MASK_LAYER_COUNT>& get_collision_mask() const;

private:
	void project_vertices(std::vector<vec2f>& vertices, const vec2f& axis, float& min, float& max);
	void find_bounding_box(Rectangle& bbox);

private:
	ShapeComponent* s_component;
	std::vector<CollisionInformation*> collision_informations;
	bool has_collided = false;

	std::bitset<COLLISION_MASK_LAYER_COUNT> collision_mask;


};

class DisplayComponent : public Component
{

public:

	DisplayComponent(ShapeComponent* s_component);
	
	void on_draw_call(Window* window, Renderer* renderer) override;
	vec4f color;
	bool visible = true;
private:
	ShapeComponent* s_component;


};


