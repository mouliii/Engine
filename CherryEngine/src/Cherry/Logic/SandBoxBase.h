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
	virtual void on_loop_end() {}


protected:

	friend class CherryEngine;



	
};

