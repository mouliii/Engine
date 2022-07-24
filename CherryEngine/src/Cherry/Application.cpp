#include "Application.h"


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

int CherryEngine::i_push_layer(Layer* layer)
{
	layer->on_init();
	layer_manager->push_layer(layer);
	return 0;
}

int CherryEngine::i_run()
{
	

	while (!window->window_should_close())
	{
		renderer->clear();
		renderer->begin_batch();
		window->poll_events();

		auto& io = ImGui::GetIO();
		if (!(io.WantCaptureMouse || io.WantCaptureKeyboard))
		{
			EventVector events = window->get_events();
			if (events.size())
			{
				for (Event& e : events)
				{
					
					for (auto* layer : layer_manager->layers)
					{
						if (e.is_handled) break;
						layer->on_event(e);
					}
				}
			}	
		}
		
		dt = window->get_timestep();
	
		for (auto* layer : layer_manager->layers)
		{
			layer->on_update(dt);
		}
		
		renderer->end_batch();
		// imgui
		for (auto* layer : layer_manager->layers)
		{
			layer->on_loop_end();
		}
		// full frame swap - draw
		glfwSwapBuffers(window->_get_glfw_window());
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




Window* CherryEngine::i_get_render_window() const
{
	return window;
}

Renderer* CherryEngine::i_get_renderer() const
{
	return renderer;
}

OrthoCamera* CherryEngine::i_get_ortho_camera() const
{
	return orthocamera;
}

ECSManager* CherryEngine::i_get_manager() const
{
	return manager;
}
