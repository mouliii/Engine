#pragma once
#include <unordered_map>
#include <memory>


#include "SandBoxBase.h"

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
	static int add_layer(SandBoxBase* sandbox_layer, const std::string& name) { return get().i_add_layer(sandbox_layer, name); }
	static int run() { return get().i_run(); }
	static int destroy() { return get().i_destroy(); }

	static const SandBoxBase* get_layer(const std::string& name) { return get().i_get_layer(name); }
	static const Window* get_render_window() { return get().i_get_render_window(); }

private:

	int i_init();
	int i_add_layer(SandBoxBase* sandbox_layer, std::string name);
	int i_run();
	int i_destroy();

	const SandBoxBase* i_get_layer(const std::string& name);
	const Window* i_get_render_window() const;

	CherryEngine() {}

	bool initialized = false;

	float dt;

	std::unordered_map<std::string, SandBoxBase*> layers;

	Window* window;
	Renderer* renderer;
	OrthoCamera* orthocamera;
};