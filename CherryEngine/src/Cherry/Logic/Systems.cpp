#include "Systems.h"

void DrawSystem::on_draw_call(Window* render_window, Renderer* renderer)
{
	for (const auto& entity : entities)
	{
		auto& display_component = CherryEngine::get_manager()->get_component<DisplayComponent>(entity);
		if (!display_component.visible) continue;
		auto& shape_component = CherryEngine::get_manager()->get_component<ShapeComponent>(entity);

		std::vector<vec2f> points;
		shape_component.shape.get_global_points(points);
		renderer->draw_quad(points, display_component.color);
	}

}

void CollisionSystem::on_game_tick(TimeStep delta_time)
{
	for (const auto& entity : entities)
	{
		for (auto* collision : CherryEngine::get_manager()->get_component<CollisionComponent>(entity).collision_informations)
		{
			move_and_collide(entity, collision->resolution);
			delete collision;
		}
		CherryEngine::get_manager()->get_component<CollisionComponent>(entity).collision_informations.clear();
	}

}

bool CollisionSystem::AABB(Rectangle& a, Rectangle& b)
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

bool CollisionSystem::move_and_collide(Entity entity, vec2f translation)
{

	CollisionComponent* cc1 = &CherryEngine::get_manager()->get_component<CollisionComponent>(entity);
	cc1->s_component->move(translation);
	cc1->has_collided = false;
	for (const auto& entity2 : entities)
	{

		if (entity == entity2) continue;


		//if (!entity2->has_component<CollisionComponent>())
		//	continue;

		CollisionComponent* cc2 = &CherryEngine::get_manager()->get_component<CollisionComponent>(entity2);


		if (!same_bit_comparison(cc2->collision_mask, cc1->collision_mask))
			continue;

		Rectangle rect1;
		find_bounding_box(cc1, rect1);

		Rectangle rect2;
		find_bounding_box(cc2, rect2);

		if (!AABB(rect1, rect2))
			continue;

		vec2f normal;
		float depth;
		if (polygon_intersect(cc1, cc2, normal, depth))
		{
			cc1->s_component->move(-normal * depth * 0.5f);
			cc1->has_collided = true;

			cc2->collision_informations.push_back(new CollisionInformation{ normal * depth * 0.5f });
			cc2->has_collided = true;
		
			std::cout << CherryEngine::get_manager()->get_entity_name(entity) << " " << CherryEngine::get_manager()->get_entity_name(entity2) << " collided!\n";
			


		}
	}
	return cc1->has_collided;

}

void CollisionSystem::project_vertices(std::vector<vec2f>& vertices, const vec2f& axis, float& min, float& max)
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

bool CollisionSystem::polygon_intersect(CollisionComponent* a, CollisionComponent* b, vec2f& normal, float& depth)
{
	normal = { 0.f, 0.f };
	depth = INFINITY;
	int8_t negative = 1;
	std::vector<vec2f> p1;
	std::vector<vec2f> p2;

	a->s_component->shape.get_global_points(p1);
	b->s_component->shape.get_global_points(p2);

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

	return true;
}

void CollisionSystem::find_bounding_box(CollisionComponent* collision_component, Rectangle& bbox)
{
	std::vector<vec2f> points;
	collision_component->s_component->shape.get_global_points(points);

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

vec2f CollisionSystem::find_center(const std::vector<vec2f>& vertices)
{
	vec2f v;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		v += vertices[i];
	}

	v /= vertices.size();
	return v;

}
