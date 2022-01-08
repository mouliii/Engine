#include "CrEngine.h"

int CherryEngine::i_init()
{
	if (initialized) return 1;


	window = new Window();
	renderer = new Renderer();
	orthocamera = new OrthoCamera(0.f, 800.f, 0.f, 600.f);
	renderer->set_camera(orthocamera);
	initialized = true;
	return 0;
}

int CherryEngine::i_add_layer(SandBoxBase* sandbox_layer, std::string name)
{
	sandbox_layer->on_init();
	layers[name] = sandbox_layer;
	return 0;
}

int CherryEngine::i_run()
{
	while (!window->window_should_close())
	{
		glfwSwapInterval(0);
		EventVector events = window->get_events();
		if (events.size())
		{
			for (auto& it : layers)
				it.second->on_game_event(window->get_events());
		}
		dt = window->get_timestep();
		for (auto& it : layers)
		{
			it.second->update_manager(dt);
			it.second->on_game_tick(dt);

		}


		for (auto& it : layers)
		{
			it.second->on_draw_call(window, renderer);
			it.second->draw_manager(window, renderer);
		}
	}
	return 0;
}

int CherryEngine::i_destroy()
{
	delete window;
	delete renderer;
	delete orthocamera;
	for (auto& it : layers)
	{
		delete it.second;
	}


	return 0;
}

const SandBoxBase* CherryEngine::i_get_layer(const std::string& name)
{
	if (layers.find(name) == layers.end())
		return nullptr;
	return layers[name];
}

const Window* CherryEngine::i_get_render_window() const
{
	return window;
}
