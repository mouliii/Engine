#pragma once

#include "Layer.h"

class LayerManager
{
public:

	LayerManager()=default;
	~LayerManager();

	void push_layer(Layer* layer);
	void pop_layer(Layer* layer);

	std::vector<Layer*> layers;
	
};

