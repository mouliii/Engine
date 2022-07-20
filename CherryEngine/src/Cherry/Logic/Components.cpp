#include "Components.h"


CollisionComponent::CollisionComponent(ShapeComponent* s_component)
	:
	s_component(s_component)
{
	collision_mask.set();
	c_type = ComponentType::CollisionComponent;
}

void CollisionComponent::on_update_tick(TimeStep dt)
{
	if (has_collided)
	{
		for (auto* collision : collision_informations)
		{
			move_and_collide(collision->resolution);
			delete collision;
		}
		collision_informations.clear();
	}
}

bool CollisionComponent::AABB(Rectangle& a, Rectangle& b)
{
	vec2f p1 = a.get_position();
	vec2f p2 = b.get_position();

	vec2f s1 = a.get_size();
	vec2f s2 = b.get_size();
	
	return (p1.x < p2.x + s2.x &&
			p1.x + s1.x > p2.x &&
			p1.y < p2.y + s2.y &&
			p1.y + s1.y > p2.y);
	
}

bool CollisionComponent::move_and_collide(const vec2f& translation)
{

	SandBoxBase* sb = CherryEngine::get_layer("main");
	Manager* manager = sb->get_manager();
	const auto& entities = manager->get_entities();

	s_component->move(translation);
	has_collided = false;
	for (size_t i = 0; i < entities.size(); i++)
	{
		
		if (entity != entities[i].get())
		{

			CollisionComponent* other = &(entities[i]->get_component<CollisionComponent>());

			if (!other)
				continue;


			if (!same_bit_comparison(other->collision_mask, this->collision_mask))
				continue;

			Rectangle rect1;
			find_bounding_box(rect1);

			Rectangle rect2;
			entities[i]->get_component<CollisionComponent>().find_bounding_box(rect2);

			if (!AABB(rect1, rect2))
				continue;

			vec2f normal;
			float depth;
			if (polygon_intersect(&(entities[i]->get_component<CollisionComponent>()), normal, depth))
			{
				s_component->move(-normal * depth * 0.5f);
				entities[i]->get_component<CollisionComponent>().collision_informations.push_back(new CollisionInformation{ normal * depth * 0.5f});
				entities[i]->get_component<CollisionComponent>().has_collided = true;
				has_collided = true;
			}
		}
	}


	return false;

}

bool CollisionComponent::polygon_intersect(CollisionComponent* other, vec2f& normal, float& depth)
{
	normal = { 0.f, 0.f };
	depth = INFINITY;
	int8_t negative = 1;
	std::vector<vec2f> p1;
	std::vector<vec2f> p2;

	s_component->get_shape()->get_global_points(p1);
	other->s_component->get_shape()->get_global_points(p2);

	for (size_t i = 0; i < p1.size(); i++)
	{

		vec2f va = p1[i];
		vec2f vb = p1[(i + 1) % p1.size()];

		vec2f edge = vb - va;
		vec2f axis = { -edge.y, edge.x };

		float min_a, max_a, min_b, max_b;

		project_vertices(p1, axis, min_a, max_a);
		project_vertices(p2, axis, min_b, max_b);

		if ((min_a >= max_b) || (min_b >= max_a))
			return false;

		float axis_depth = std::min(max_b - min_a, max_a - min_b);
		
		

		if (axis_depth < depth)
		{
			if (max_b - min_a < max_a - min_b)
				negative = -1;
			else
				negative = 1;
			depth = axis_depth;
			normal = axis;
		}
	}

	for (size_t i = 0; i < p2.size(); i++)
	{

		vec2f va = p2[i];
		vec2f vb = p2[(i + 1) % p2.size()];

		vec2f edge = vb - va;
		vec2f axis = { -edge.y, edge.x };

		float min_a, max_a, min_b, max_b;

		project_vertices(p1, axis, min_a, max_a);
		project_vertices(p2, axis, min_b, max_b);

		if ((min_a >= max_b) || (min_b >= max_a))
			return false;

		float axis_depth = std::min(max_b - min_a, max_a - min_b);
		
		
		


		if (axis_depth < depth)
		{
			if (max_b - min_a < max_a - min_b)
				negative = -1;
			else
				negative = 1;
			depth = axis_depth;
			normal = axis;
		}
	}

	

	depth /= normal.lenght();
	depth *= negative;
	normal.normalize();

	//vec2f center_a = find_center(p1);
	//vec2f center_b = find_center(p2);
	//
	//vec2f dir = center_b - center_a;
	//
	//if (dir.dot(normal) < 0.f)
	//{
	//	normal = -normal;
	//}

	

	return true;
}

void CollisionComponent::project_vertices(std::vector<vec2f>& vertices, const vec2f& axis, float& min, float& max)
{

	min = INFINITY;
	max = -INFINITY;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		float projection = vertices[i].dot(axis);
		if (projection < min)
			min = projection;
		if (projection > max)
			max = projection;
	}

}

vec2f CollisionComponent::find_center(std::vector<vec2f>& vertices)
{
	vec2f v;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		v += vertices[i];
	}

	v /= vertices.size();
	return v;
	
}

void CollisionComponent::flip_collision_mask_bit(uint32_t bit_index)
{
	collision_mask.flip(bit_index);
}

void CollisionComponent::set_collision_mask_bit(uint32_t bit_index, bool b)
{
	collision_mask.set(bit_index, b);
}

const std::bitset<COLLISION_MASK_LAYER_COUNT>& CollisionComponent::get_collision_mask() const
{
	return collision_mask;
}

void CollisionComponent::find_bounding_box(Rectangle& bbox)
{
	std::vector<vec2f> points;
	s_component->get_shape()->get_global_points(points);

	float left = INFINITY;
	float right = -INFINITY;
	
	float top = -INFINITY;
	float bottom = INFINITY;
	for (size_t i = 0; i < points.size(); i++)
	{
		const vec2f& p = points[i];
		if (p.x < left)
			left = p.x;
		if (p.x > right)
			right = p.x;

		if (p.y < bottom)
			bottom = p.y;
		if (p.y > top)
			top = p.y;
	}

	bbox.set_local_points(
		{
			{left, bottom},
			{left, top},
			{right, top},
			{right, bottom}
		}
	);
}



void ShapeComponent::move(vec2f translation)
{
	shape.translate(translation);
}




ShapeComponent::ShapeComponent(Polygon shape)
	:
	shape(shape)
{
	c_type = ComponentType::ShapeComponent;
}

Polygon* ShapeComponent::get_shape()
{
	return &shape;
}


DisplayComponent::DisplayComponent(ShapeComponent* s_component)
	:
	s_component(s_component),
	color(1.f, 1.f, 1.f, 1.f)
{
	c_type = ComponentType::DisplayComponent;
}

void DisplayComponent::on_draw_call(Window* window, Renderer* renderer)
{
	if (visible)
	{
		std::vector<vec2f> points;
		s_component->get_shape()->get_global_points(points);

		//renderer->draw_quad(points, color);
		renderer->DrwaQuad(points, color);
	}
}