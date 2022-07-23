#pragma once
#include "../CherryI.h"

class Layer
{
public:

	Layer() {}
	~Layer() { on_destruct(); }

	virtual void on_init() {}
	virtual void on_update(TimeStep delta_time) {}
	virtual void on_event(Event& event) {}
	virtual void on_loop_end() {}
	virtual void on_destruct() {}

	protected:
		friend class CherryEngine;

};


