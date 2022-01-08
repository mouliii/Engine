#include "Cherry.h"

class PositionComponent : public Component
{
public:
	PositionComponent(float x, float y)
		:
		pos(x, y)
	{

	}

	void on_init() override
	{
		std::cout << pos << std::endl;
	}

	vec2f pos;

};

class PlayerEntity : public Entity
{
public:

	void on_init() override
	{

	}
	void on_update_tick(TimeStep dt) override
	{

		if (KeyBoard::is_key_pressed(GLFW_KEY_A, CherryEngine::get_render_window()))
			this->get_component<PositionComponent>().pos.x -= 1000.f * dt;
		if (KeyBoard::is_key_pressed(GLFW_KEY_D, CherryEngine::get_render_window()))
			this->get_component<PositionComponent>().pos.x += 1000.f * dt;
		if (KeyBoard::is_key_pressed(GLFW_KEY_W, CherryEngine::get_render_window()))
			this->get_component<PositionComponent>().pos.y += 1000.f * dt;
		if (KeyBoard::is_key_pressed(GLFW_KEY_S, CherryEngine::get_render_window()))
			this->get_component<PositionComponent>().pos.y -= 1000.f * dt;
	}

	void on_draw_call(Window* render_window, Renderer* renderer) override
	{
		OrthoCamera* ortho_cam = renderer->get_camera();

		ortho_cam->set_rotation(0.01f + ortho_cam->get_rotation());
		ortho_cam->set_position(this->get_component<PositionComponent>().pos - vec2f{400.f, 300.f});
		renderer->update_camera();
		renderer->draw_quad(this->get_component<PositionComponent>().pos, { 40.f, 40.f }, { 0.4f, 0.1f, 0.7f, 1.f });

	}


};


class SandboxLayer : public SandBoxBase
{
public:

	virtual void on_init() override
	{
		//printf("INITED\n");
		player_object = &manager.add_entity<PlayerEntity>();
		player_object->add_component<PositionComponent>(50.f, 20.f);
	}
	virtual void on_game_tick(TimeStep delta_time) override
	{
		//printf("GAME_TICKED\n");


	}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) override
	{
		printf("%f fps\n", render_window->get_fps());

		renderer->display();
		//printf("DRAW_CALLED\n");
	}
	virtual void on_game_event(EventVector events) override
	{

		for (auto& e : events)
		{
			switch (e.type)
			{
			case EventType::KEYBOARD:
			{
				if (e.keyboard_ev.key == GLFW_KEY_A)
					player_object->get_component<PositionComponent>().pos.x -= 10.f; break;
				break;
			}

			default:
				break;
			}
		}
		//printf("EVENT_CALLED\n");
	}

private:
	PlayerEntity* player_object;

};


int main()
{

	CherryEngine::init();
	
	SandboxLayer* sb_layer = new SandboxLayer();
	SandboxLayer* sb_layer2 = new SandboxLayer();
	
	CherryEngine::add_layer(sb_layer, "main");
	
	std::cout << CherryEngine::get_layer("main") << " " << CherryEngine::get_layer("secondary") << "\n";
	
	CherryEngine::run();
	CherryEngine::destroy();

}


 