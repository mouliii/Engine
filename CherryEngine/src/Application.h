#pragma once
#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"


class Application : public SandBoxBase
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
		shape.shape.get_global_points(points);
		const_cast<OrthoCamera*>(ortho_cam)->set_position(system->find_center(points) - window->get_sizef() * 0.5f);



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
				ortho_cam->resize(0, e.frame_buf_res_ev.w, 0, e.frame_buf_res_ev.h);
				break;
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


