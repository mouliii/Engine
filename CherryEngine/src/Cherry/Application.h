#pragma once
#include <unordered_map>
#include <memory>

#include "Logic/LayerManager.h"
#include "Logic/Components.h"
#include "Logic/EntityComponentSystem.h"

#include "Physics/Shape/Polygon.h"
#include "../../imgui/imgui.h"

class CherryEngine
{
public:


	CherryEngine(const CherryEngine&) = delete;

	static CherryEngine& get()
	{
		static CherryEngine instance;
		return instance;
	}



	static int init() { return get().i_init(); }
	static int push_layer(Layer* layer) { return get().i_push_layer(layer); }
	static int destroy_layer(Layer* layer) { return get().i_destroy_layer(layer); }
	static int run() { return get().i_run(); }
	static int destroy() { return get().i_destroy(); }

	static Window* get_render_window() { return get().i_get_render_window(); }
	static Renderer* get_renderer() { return get().i_get_renderer(); }
	static OrthoCamera* get_ortho_camera() { return get().i_get_ortho_camera(); }
	static ECSManager* get_manager() { return get().i_get_manager(); }

	

	
	
private:

	int i_init();
	int i_push_layer(Layer* layer);
	int i_run();
	int i_destroy();
	int i_destroy_layer(Layer* layer);

	Window* i_get_render_window() const;
	Renderer* i_get_renderer() const;
	OrthoCamera* i_get_ortho_camera() const;
	ECSManager* i_get_manager() const;


	CherryEngine() {}


private:

	bool initialized = false;
	float dt;
	

	Window* window;

	Renderer* renderer;
	OrthoCamera* orthocamera;
	ECSManager* manager;

	LayerManager* layer_manager;
}; 