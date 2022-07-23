#pragma once
#include <algorithm>
#include <unordered_map>

#include "../Maths/BitSetLogic.h"
#include "../Physics/Shape/Rectangle.h"

#include "../Application.h"

#define COLLISION_MASK_LAYER_COUNT 20


struct ShapeComponent
{
	Polygon shape;

	void move(vec2f translation);

};
struct CollisionInformation
{
	vec2f resolution;
};
struct CollisionComponent
{
public:
	

	ShapeComponent* s_component;
	std::vector<CollisionInformation*> collision_informations;
	bool has_collided = false;
	std::bitset<COLLISION_MASK_LAYER_COUNT> collision_mask;


	void flip_collision_mask_bit(uint32_t bit_index);
	void set_collision_mask_bit(uint32_t bit_index, bool b);
	void flip_all_bits() { collision_mask.flip(); }

};

struct DisplayComponent
{
	ShapeComponent* s_component;
	vec4f color;
	bool visible;
};


