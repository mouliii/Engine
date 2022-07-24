#pragma once
#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"


class ExampleLayer : public Layer
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

		//tri.push_back({ 40.f, -50.f });


		Entity triangle = CherryEngine::get_manager()->create_entity("triangle");
		CherryEngine::get_manager()->add_component(triangle, ShapeComponent{ Polygon(tri) });
		CherryEngine::get_manager()->add_component(triangle, CollisionComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(triangle) });
		CherryEngine::get_manager()->add_component(triangle, DisplayComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(triangle), {1.f, 1.f, 1.f, 1.f}, true });

		CherryEngine::get_manager()->get_component<CollisionComponent>(triangle).flip_all_bits();
		//	CherryEngine::get_manager()->get_component<Col 



		player_object = CherryEngine::get_manager()->create_entity("player");
		CherryEngine::get_manager()->add_component(player_object, ShapeComponent{ Rectangle({ -25.f, -25.f }, { 50.f, 50.f }) });
		CherryEngine::get_manager()->add_component(player_object, CollisionComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object) });
		CherryEngine::get_manager()->add_component(player_object, DisplayComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object), {1.f, 1.f, 1.f, 1.f}, true });

		CherryEngine::get_manager()->get_component<CollisionComponent>(player_object).flip_all_bits();
		//player_object->get_component<CollisionComponent>().flip_collision_mask_bit(0);
		//player_object->get_component<CollisionComponent>().flip_collision_mask_bit(1);
		
		for (size_t i = 0; i < 100; i++)
		{
			Entity triangle = CherryEngine::get_manager()->create_entity("triangle");
			CherryEngine::get_manager()->add_component(triangle, ShapeComponent{ Rectangle(
				{(float)(rand() % 4000 - 2000), (float)(rand() % 4000 - 2000)},
				{(float)(rand() % 200), (float)(rand() % 200)}
				) });
			//CherryEngine::get_manager()->add_component(triangle, CollisionComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object) });
			CherryEngine::get_manager()->add_component(triangle, DisplayComponent{ &CherryEngine::get_manager()->get_component<ShapeComponent>(player_object), {1.f, 1.f, 1.f, 1.f}, true });
		}
	}
	virtual void on_update(TimeStep delta_time) override
	{

		CollisionComponent& collider = CherryEngine::get_manager()->get_component<CollisionComponent>(player_object);
		//ShapeComponent& shape = player_object->get_component<ShapeComponent>();
		ShapeComponent& shape = CherryEngine::get_manager()->get_component<ShapeComponent>(player_object);
		CollisionSystem* system = static_cast<CollisionSystem*>(CherryEngine::get_manager()->get_system_manager()->get_system<CollisionSystem>().get());


		system->on_game_tick(delta_time);

		if (KeyBoard::is_key_pressed(GLFW_KEY_S, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, -500.f * delta_time));
		if (KeyBoard::is_key_pressed(GLFW_KEY_W, window))
			system->move_and_collide(player_object, mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 500.f * delta_time));
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
		shape.shape.get_global_points();
		const_cast<OrthoCamera*>(ortho_cam)->set_position(system->find_center(points) - window->get_sizef() * 0.5f);


		CherryEngine::get_renderer()->update_camera();
		
		DrawSystem* dsystem = static_cast<DrawSystem*>(CherryEngine::get_manager()->get_system_manager()->get_system<DrawSystem>().get());
		dsystem->on_draw_call(CherryEngine::get_render_window(), CherryEngine::get_renderer());
		
	}

	bool on_framebuffer_resize(Event& e)
	{
 		ortho_cam->resize(0, (float)e.frame_buf_res_ev.w, 0, (float)e.frame_buf_res_ev.h);
		return true;
	}

	virtual void on_event(Event& e) override
	{
		EventDispatcher frame_buffer_resize(e);
	//	auto fn = std::bind(&ExampleLayer::on_framebuffer_resize, *this, e);
		frame_buffer_resize.dispatch_event<EventType::FRAMEBUFFER_RESIZE>(BIND_EVENT_FN(ExampleLayer::on_framebuffer_resize));
	
	
		switch (e.type)
		{

		case EventType::SCROLL:
		{

			//std::cout << e.scroll_ev.y_offset << std::endl;
			break;
		}

		case EventType::WINDOW_RESIZE:
		{

			////window->set_size(e.window_res_ev.w, e.window_res_ev.h);
			//std::cout << e.window_res_ev.w << " " << e.window_res_ev.h << std::endl;
			//ortho_cam->resize(0, e.window_res_ev.w, 0, e.window_res_ev.h);
			break;
		}

		case EventType::FRAMEBUFFER_RESIZE:
		{
			//std::cout << e.frame_buf_res_ev.w << " " << e.frame_buf_res_ev.h << std::endl;
			
		}

		}
	}

	



private:
	Entity player_object;
	Entity obstacle;

	Window* window;
	OrthoCamera* ortho_cam;



};


