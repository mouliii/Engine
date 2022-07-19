#include "CrEngine.h"


int CherryEngine::i_init()
{
	if (initialized) return 1;

	window = new Window();

	manager = new ECSManager();
	
	glfwSwapInterval(0);
	renderer = new Renderer();
	orthocamera = new OrthoCamera(0.f, 800.f, 0.f, 600.f);
	renderer->set_camera(orthocamera);
	initialized = true;



	/* IMGUI INITIALIZATION */

	

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

		//EventVector events = window->get_events();

		window->poll_events();
	   
	
	   
		/*
		if (events.size())
		{
			for (auto& it : layers)
				it.second->on_game_event(events);
		}
		*/

		dt = window->get_timestep();
		for (auto& it : layers)
		{
			it.second->on_game_tick(dt);
		}


		for (auto& it : layers)
		{
			it.second->on_draw_call(window, renderer);
		}

	
		


		renderer->display();
		
		for (auto& it : layers)
		{
			it.second->on_loop_end();
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

SandBoxBase* CherryEngine::i_get_layer(const std::string& name)
{
	if (layers.find(name) == layers.end())
		return nullptr;
	return layers[name];
}

Window* CherryEngine::i_get_render_window() const
{
	return window;
}

OrthoCamera* CherryEngine::i_get_ortho_camera() const
{
	return orthocamera;
}

ECSManager* CherryEngine::i_get_manager() const
{
	return manager;
}
