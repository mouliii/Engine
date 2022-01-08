#pragma once

#include "EntityComponentSystem.h"

/*
Baseclass for layouts for the engine to call.
*/
class SandBoxBase
{
public:
	
	virtual void on_init() {}
	virtual void on_game_tick(TimeStep delta_time) {}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) {}
	virtual void on_game_event(EventVector events) {}

private:

	friend class CherryEngine;
	virtual void update_manager(TimeStep dt) final
	{
		manager.refresh();
		manager.update(dt);
	}

	virtual void draw_manager(Window* render_window, Renderer* renderer) final
	{
		manager.draw(render_window, renderer);
	}

protected:
	Manager manager;
};

