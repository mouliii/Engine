#include "Components.h"




void CollisionComponent::flip_collision_mask_bit(uint32_t bit_index)
{
	collision_mask.flip(bit_index);
}

void CollisionComponent::set_collision_mask_bit(uint32_t bit_index, bool b)
{
	collision_mask.set(bit_index, b);
}


void ShapeComponent::move(vec2f translation)
{
	shape.translate(translation);
}
