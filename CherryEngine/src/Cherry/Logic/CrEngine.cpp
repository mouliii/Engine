#include "CrEngine.h"


int CherryEngine::i_init()
{
	if (initialized) return 1;

	window = new Window();
	manager = new ECSManager();
	layer_manager = new LayerManager();

	glfwSwapInterval(0);
	renderer = new Renderer();
	orthocamera = new OrthoCamera(0.f, 800.f, 0.f, 600.f);
	renderer->set_camera(orthocamera);
	initialized = true;

	return 0;
}

int CherryEngine::i_add_layer(Layer* layer)
{
	layer_manager->push_layer(layer);
	return 0;
}

int CherryEngine::i_run()
{
	

	while (!window->window_should_close())
	{

		

		window->poll_events();
		
		auto& io = ImGui::GetIO();
		if (!(io.WantCaptureMouse || io.WantCaptureKeyboard))
		{
			EventVector events = window->get_events();
			if (events.size())
			{
				for (auto* application : applications)
				{
					application->on_game_event(events);
				}
				for (auto* layer : layer_manager->layers)
				{
					layer->on_event(events);
				}
			}	
		}

		dt = window->get_timestep();
		for (auto* application : applications)
		{
			application->on_game_tick(dt);
		}
		for (auto* layer : layer_manager->layers)
		{
			layer->on_update(dt);
		}


		for (auto* application : applications)
		{
			application->on_draw_call(window, renderer);
		}
		renderer->display();
		
		for (auto* application : applications)
		{
			application->on_loop_end();
		}


	}
	return 0;
}

int CherryEngine::i_destroy()
{
	delete window;
	delete renderer;
	delete orthocamera;
	delete layer_manager;


	return 0;
}

int CherryEngine::i_destroy_layer(Layer* layer)
{

	layer_manager->pop_layer(layer);
	return 0;
}

int CherryEngine::i_add_application(SandBoxBase* application)
{
	application->on_init();
	applications.push_back(application);
	return 0;
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
