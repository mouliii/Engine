#pragma once
#include <unordered_map>
#include <memory>

#include "SandBoxBase.h"
#include "LayerManager.h"
#include "Components.h"
#include "EntityComponentSystem.h"

#include "../Physics/Shape/Polygon.h"
#include "../../../imgui/imgui.h"

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
	static int push_layer(Layer* layer) { return get().i_add_layer(layer); }
	static int destroy_layer(Layer* layer) { return get().i_destroy_layer(layer); }
	static int push_application(SandBoxBase* application) { return get().i_add_application(application); }
	static int run() { return get().i_run(); }
	static int destroy() { return get().i_destroy(); }

	static Window* get_render_window() { return get().i_get_render_window(); }
	static OrthoCamera* get_ortho_camera() { return get().i_get_ortho_camera(); }
	static ECSManager* get_manager() { return get().i_get_manager(); }
	

	
	
private:

	int i_init();
	int i_add_layer(Layer* layer);
	int i_run();
	int i_destroy();
	int i_destroy_layer(Layer* layer);
	int i_add_application(SandBoxBase* application);

	Window* i_get_render_window() const;
	OrthoCamera* i_get_ortho_camera() const;
	ECSManager* i_get_manager() const;

	CherryEngine() {}


private:

	bool initialized = false;
	float dt;
	
	std::vector<SandBoxBase*> applications;

	Window* window;

	Renderer* renderer;
	OrthoCamera* orthocamera;
	ECSManager* manager;

	LayerManager* layer_manager;
}; 