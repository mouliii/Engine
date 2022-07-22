#pragma once
#include "../CherryI.h"

class Layer
{
public:

	Layer() { on_init(); }
	~Layer() { on_destruct(); }

	virtual void on_init() {}
	virtual void on_update(TimeStep delta_time) {}
	virtual void on_event(EventVector events) {}
	virtual void on_destruct() {}

	protected:
		friend class CherryEngine;

};


