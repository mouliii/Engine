#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"


class SandboxLayer : public SandBoxBase
{
public:


	virtual void on_init() override
	{

		window = CherryEngine::get_render_window();
		ortho_cam = CherryEngine::get_ortho_camera();

	
		

		//std::vector<vec2f> tri;
		//tri.push_back({ 10.f, 10.f });
		//tri.push_back({ 80.f, 80.f });
		//tri.push_back({ 90.f, 10.f });
		//tri.push_back({ 70.f, -40.f });

		//tri.push_back({ 40.f, -50.f });


		//Entity* triangle = &manager.add_entity("triangle");
		//triangle->add_component<ShapeComponent>(Polygon(tri));
		//triangle->add_component<DisplayComponent>(&triangle->get_component<ShapeComponent>());
		//triangle->add_component<CollisionComponent>(&triangle->get_component<ShapeComponent>());

		//triangle->get_component<CollisionComponent>().flip_all_bits();
		//triangle->get_component<CollisionComponent>().flip_collision_mask_bit(0);


		

		player_object = &manager.add_entity("player");
		player_object->add_component<ShapeComponent>(Rectangle( { -25.f, -25.f }, {50.f, 50.f} ));
		player_object->add_component<DisplayComponent>(&player_object->get_component<ShapeComponent>());
		player_object->add_component<CollisionComponent>(&player_object->get_component<ShapeComponent>());

		player_object->get_component<CollisionComponent>().flip_all_bits();
		player_object->get_component<CollisionComponent>().flip_collision_mask_bit(0);
		player_object->get_component<CollisionComponent>().flip_collision_mask_bit(1);



		obstacle = &manager.add_entity("obs_1");
		obstacle->add_component<ShapeComponent>(Rectangle{ {100.f, 200.f}, {200.f, 100.f} });
		obstacle->add_component<DisplayComponent>(&obstacle->get_component<ShapeComponent>());
		//obstacle->add_component<CollisionComponent>(&obstacle->get_component<ShapeComponent>());

		//obstacle->get_component<CollisionComponent>().flip_all_bits();
		//obstacle->get_component<CollisionComponent>().flip_collision_mask_bit(1);

		Entity* obstacle_2 = &manager.add_entity("obs_2");
		obstacle_2->add_component<ShapeComponent>(Rectangle{ { -500.f, -500.f}, {1000.f, 50.f} });
		obstacle_2->add_component<DisplayComponent>(  &obstacle_2->get_component<ShapeComponent>());
		//obstacle_2->add_component<CollisionComponent>(&obstacle_2->get_component<ShapeComponent>());

		Entity* obstacle_3 = &manager.add_entity("obs_3");
		obstacle_3->add_component<ShapeComponent>(Rectangle{ {500.f, -500.f}, {50.f, 1000.f} });
		obstacle_3->add_component<DisplayComponent>(&  obstacle_3->get_component<ShapeComponent>());
		obstacle_3->add_component<CollisionComponent>(&obstacle_3->get_component<ShapeComponent>());

		Entity* obstacle_4 = &manager.add_entity("obs_4");
		obstacle_4->add_component<ShapeComponent>(Rectangle{ { -500.f, 500.f}, {1000.f, 50.f} });
		obstacle_4->add_component<DisplayComponent>(&  obstacle_4->get_component<ShapeComponent>());
		obstacle_4->add_component<CollisionComponent>(&obstacle_4->get_component<ShapeComponent>());

		Entity* obstacle_5 = &manager.add_entity("obs_5");
		obstacle_5->add_component<ShapeComponent>(Rectangle{ { -500.f, -500.f}, {50.f, 1000.f} });
		obstacle_5->add_component<DisplayComponent>(&  obstacle_5->get_component<ShapeComponent>());
		obstacle_5->add_component<CollisionComponent>(&obstacle_5->get_component<ShapeComponent>());

		/*
		for (size_t i = 0; i < 100; i++)
		{
			Entity* obstacle_5 = &manager.add_entity();
			obstacle_5->add_component<ShapeComponent>(Rectangle{ { (float)(rand() % 4000 - 2000), (float)(rand() % 4000 - 2000)}, {(float)(rand() % 200), (float)(rand() % 200)}});
			obstacle_5->add_component<DisplayComponent>(&obstacle_5->get_component<ShapeComponent>());
			obstacle_5->add_component<CollisionComponent>(&obstacle_5->get_component<ShapeComponent>());
		}
		*/

		

		
	}  
	virtual void on_game_tick(TimeStep delta_time) override
	{
		

		
		CollisionComponent& collider = player_object->get_component<CollisionComponent>();
		ShapeComponent& shape = player_object->get_component<ShapeComponent>();

		
			

		if (KeyBoard::is_key_pressed(GLFW_KEY_S, window))
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, -500.f * delta_time));
		if (KeyBoard::is_key_pressed(GLFW_KEY_W, window))
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 500.f*delta_time));
		if (KeyBoard::is_key_pressed(GLFW_KEY_D, window))
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(500.f * delta_time, 0.f));
		if (KeyBoard::is_key_pressed(GLFW_KEY_A, window))
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(-500.f * delta_time, 0.f));

		if (KeyBoard::is_key_pressed(GLFW_KEY_Q, window))
		{
			shape.get_shape()->rotate(delta_time);
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
			//const_cast<OrthoCamera*>(ortho_cam)->set_rotation(ortho_cam->get_rotation() + delta_time);
		}
		if (KeyBoard::is_key_pressed(GLFW_KEY_E, window))
		{
			shape.get_shape()->rotate(-delta_time);
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
			//const_cast<OrthoCamera*>(ortho_cam)->set_rotation(ortho_cam->get_rotation() - delta_time);
		}
		if (KeyBoard::is_key_pressed(GLFW_KEY_LEFT, window))
		{
			shape.get_shape()->scale(delta_time);
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
		}
		//	const_cast<OrthoCamera*>(ortho_cam)->set_scale(ortho_cam->get_scale() + delta_time);
		if (KeyBoard::is_key_pressed(GLFW_KEY_RIGHT, window))
		{
			shape.get_shape()->scale(-delta_time);
			collider.move_and_collide(mat4f::rotate_z(ortho_cam->get_rotation()) * vec2f(0.f, 0.f));
		}
			//const_cast<OrthoCamera*>(ortho_cam)->set_scale(ortho_cam->get_scale() - delta_time);
		std::vector<vec2f> points;
		shape.get_shape()->get_global_points(points);
		const_cast<OrthoCamera*>(ortho_cam)->set_position(collider.find_center(points) - window->get_sizef() * 0.5f);

		

	}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) override
	{


	
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
	Entity* player_object;
	Entity* obstacle;

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


 