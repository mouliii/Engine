#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"


class DrawSystem : public System
{
public:
	DrawSystem() = default;

	void on_draw_call(Window* render_window, Renderer* renderer)
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
};


class CollisionSystem : public System
{
public:
	CollisionSystem() = default;

	void on_game_tick(TimeStep delta_time)
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

	bool AABB(Rectangle& a, Rectangle& b)
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


	bool move_and_collide(Entity entity, vec2f translation)
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
				
			}
		}
		return cc1->has_collided;

	}

	void project_vertices(std::vector<vec2f>& vertices, const vec2f& axis, float& min, float& max)
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

	bool polygon_intersect(CollisionComponent* a, CollisionComponent* b, vec2f& normal, float& depth)
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


	void find_bounding_box(CollisionComponent* collision_component, Rectangle& bbox)
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

	vec2f find_center(const std::vector<vec2f>& vertices)
	{
		vec2f v;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			v += vertices[i];
		}

		v /= vertices.size();
		return v;

	}
};



class SandboxLayer : public SandBoxBase
{
public:


	virtual void on_init() override
	{

		window = CherryEngine::get_render_window();
		ortho_cam = CherryEngine::get_ortho_camera();

	
		

		auto def_system = CherryEngine::get_manager()->register_system<System>();
		Signature signature_default;
		CherryEngine::get_manager()->set_system_signature<System>(signature_default);

		CherryEngine::get_manager()->register_component<ShapeComponent>();
		CherryEngine::get_manager()->register_component<DisplayComponent>();
		CherryEngine::get_manager()->register_component<CollisionComponent>();

		auto collision_system = CherryEngine::get_manager()->register_system<CollisionSystem>();

		Signature signature;
	//	signature.set(CherryEngine::get_manager()->get_component_type<ShapeComponent>());
		signature.set(CherryEngine::get_manager()->get_component_type<CollisionComponent>());

		CherryEngine::get_manager()->set_system_signature<CollisionSystem>(signature);
		

		auto draw_system = CherryEngine::get_manager()->register_system<DrawSystem>();
		Signature signature1;
		signature1.set(CherryEngine::get_manager()->get_component_type<ShapeComponent>());
		signature1.set(CherryEngine::get_manager()->get_component_type<DisplayComponent>());
		CherryEngine::get_manager()->set_system_signature<DrawSystem>(signature1);

		std::vector<vec2f> tri;
		tri.push_back({ 10.f, 10.f });
		tri.push_back({ 80.f, 80.f });
		tri.push_back({ 90.f, 10.f });
		tri.push_back({ 70.f, -40.f });

		tri.push_back({ 40.f, -50.f });


		Entity triangle = CherryEngine::get_manager()->create_entity();
		CherryEngine::get_manager()->add_component(triangle, ShapeComponent{ Polygon(tri) });
		CherryEngine::get_manager()->add_component(triangle, CollisionComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(triangle) });
		CherryEngine::get_manager()->add_component(triangle, DisplayComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(triangle), {1.f, 1.f, 1.f, 1.f}, true });

		CherryEngine::get_manager()->get_component<CollisionComponent>(triangle).flip_all_bits();
	//	CherryEngine::get_manager()->get_component<Col 
		
		

		player_object = CherryEngine::get_manager()->create_entity();
		CherryEngine::get_manager()->add_component(player_object, ShapeComponent{ Rectangle({ -25.f, -25.f }, { 50.f, 50.f }) });
		CherryEngine::get_manager()->add_component(player_object, CollisionComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object) });
		CherryEngine::get_manager()->add_component(player_object, DisplayComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object), {1.f, 1.f, 1.f, 1.f}, true });

		CherryEngine::get_manager()->get_component<CollisionComponent>(player_object).flip_all_bits();
		//player_object->get_component<CollisionComponent>().flip_collision_mask_bit(0);
		//player_object->get_component<CollisionComponent>().flip_collision_mask_bit(1);



		//obstacle = &get_manager().add_entity("obs_1");
		//obstacle->add_component<ShapeComponent>(Rectangle{ {100.f, 200.f}, {200.f, 100.f} });
		//obstacle->add_component<DisplayComponent>(&obstacle->get_component<ShapeComponent>());
		////obstacle->add_component<CollisionComponent>(&obstacle->get_component<ShapeComponent>());
		//
		////obstacle->get_component<CollisionComponent>().flip_all_bits();
		////obstacle->get_component<CollisionComponent>().flip_collision_mask_bit(1);
		//
		//Entity* obstacle_2 = &get_manager().add_entity("obs_2");
		//obstacle_2->add_component<ShapeComponent>(Rectangle{ { -500.f, -500.f}, {1000.f, 50.f} });
		//obstacle_2->add_component<DisplayComponent>(  &obstacle_2->get_component<ShapeComponent>());
		////obstacle_2->add_component<CollisionComponent>(&obstacle_2->get_component<ShapeComponent>());
		//
		//Entity* obstacle_3 = &get_manager().add_entity("obs_3");
		//obstacle_3->add_component<ShapeComponent>(Rectangle{ {500.f, -500.f}, {50.f, 1000.f} });
		//obstacle_3->add_component<DisplayComponent>(&  obstacle_3->get_component<ShapeComponent>());
		//obstacle_3->add_component<CollisionComponent>(&obstacle_3->get_component<ShapeComponent>());
		//
		//Entity* obstacle_4 = &get_manager().add_entity("obs_4");
		//obstacle_4->add_component<ShapeComponent>(Rectangle{ { -500.f, 500.f}, {1000.f, 50.f} });
		//obstacle_4->add_component<DisplayComponent>(&  obstacle_4->get_component<ShapeComponent>());
		//obstacle_4->add_component<CollisionComponent>(&obstacle_4->get_component<ShapeComponent>());
		//
		//Entity* obstacle_5 = &get_manager().add_entity("obs_5");
		//obstacle_5->add_component<ShapeComponent>(Rectangle{ { -500.f, -500.f}, {50.f, 1000.f} });
		//obstacle_5->add_component<DisplayComponent>(&  obstacle_5->get_component<ShapeComponent>());
		//obstacle_5->add_component<CollisionComponent>(&obstacle_5->get_component<ShapeComponent>());

		/*
		for (size_t i = 0; i < 100; i++)
		{
			Entity* obstacle_5 = &get_manager().add_entity();
			obstacle_5->add_component<ShapeComponent>(Rectangle{ { (float)(rand() % 4000 - 2000), (float)(rand() % 4000 - 2000)}, {(float)(rand() % 200), (float)(rand() % 200)}});
			obstacle_5->add_component<DisplayComponent>(&obstacle_5->get_component<ShapeComponent>());
			obstacle_5->add_component<CollisionComponent>(&obstacle_5->get_component<ShapeComponent>());
		}
		*/

		

		
	}  
	virtual void on_game_tick(TimeStep delta_time) override
	{
		
		
		
		CollisionComponent& collider = CherryEngine::get_manager()->get_component<CollisionComponent>(player_object);
		//ShapeComponent& shape = player_object->get_component<ShapeComponent>();
		ShapeComponent& shape = CherryEngine::get_manager()->get_component<ShapeComponent>(player_object);
		CollisionSystem* system = static_cast<CollisionSystem*>(CherryEngine::get_manager()->get_system_manager()->get_system<CollisionSystem>().get());
		
		
		if (KeyBoard::is_key_pressed(GLFW_KEY_S, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, -500.f * delta_time));
		if (KeyBoard::is_key_pressed(GLFW_KEY_W, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 500.f*delta_time));
		if (KeyBoard::is_key_pressed(GLFW_KEY_D, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(500.f * delta_time, 0.f));
		if (KeyBoard::is_key_pressed(GLFW_KEY_A, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(-500.f * delta_time, 0.f));
		
		if (KeyBoard::is_key_pressed(GLFW_KEY_Q, window))
		{
			
			shape.shape.rotate(delta_time);
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
			//const_cast<OrthoCamera*>(ortho_cam)->set_rotation(ortho_cam->get_rotation() + delta_time);
		}
		if (KeyBoard::is_key_pressed(GLFW_KEY_E, window))
		{
			shape.shape.rotate(-delta_time);
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
			//const_cast<OrthoCamera*>(ortho_cam)->set_rotation(ortho_cam->get_rotation() - delta_time);
		}
		if (KeyBoard::is_key_pressed(GLFW_KEY_LEFT, window))
		{
			shape.shape.scale(delta_time);
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
		}
		//	const_cast<OrthoCamera*>(ortho_cam)->set_scale(ortho_cam->get_scale() + delta_time);
		if (KeyBoard::is_key_pressed(GLFW_KEY_RIGHT, window))
		{
			shape.shape.scale(-delta_time);
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
		}
			//const_cast<OrthoCamera*>(ortho_cam)->set_scale(ortho_cam->get_scale() - delta_time);
		std::vector<vec2f> points;
		shape.shape.get_global_points(points);
		const_cast<OrthoCamera*>(ortho_cam)->set_position(system->find_center(points)-window->get_sizef() * 0.5f);

		

	}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) override
	{

		DrawSystem* system = static_cast<DrawSystem*>(CherryEngine::get_manager()->get_system_manager()->get_system<DrawSystem>().get());
	
		system->on_draw_call(render_window, renderer);
		renderer->update_camera();
		
		//renderer->draw_quad({ 100.f, 100.f, }, { 100.f, 50.f, }, { 1.f, 0.f, 1.f, 1.f });
		

		//printf("DRAW_CALLED\n");
	}
	virtual void on_game_event(EventVector events) override
	{
		
	
		for (auto& e : events)
		{
			
			

			switch (e.type)
			{
			
			case EventType::SCROLL:
			{

				//std::cout << e.scroll_ev.y_offset << std::endl;

			}
				
			case EventType::KEYBOARD:
			{
				
				if (e.keyboard_ev.key == GLFW_KEY_F)
				{
					
				
				}
				break;
			}
		
			default:
				break;
			}
		}
	
	}


	
private:
	Entity player_object;
	Entity obstacle;

	Window* window;
	OrthoCamera* ortho_cam;



};


int main()
{

	CherryEngine::init();
	

	SandboxLayer* sb_layer = new SandboxLayer();	
	CherryEngine::add_layer(sb_layer, "main");

	ImGuiLayer* imguil = new ImGuiLayer();
	CherryEngine::add_layer(imguil, "imguil");

	//std::cout << CherryEngine::get_layer("main") << " " << CherryEngine::get_layer("secondary") << "\n";
	

	CherryEngine::run();
	CherryEngine::destroy();

}


 