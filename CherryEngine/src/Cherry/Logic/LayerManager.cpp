#include "LayerManager.h"


LayerManager::~LayerManager()
{
	for (auto* layer : layers)
	{
		delete layer;
	}
}

void LayerManager::push_layer(Layer* layer)
{
	layers.emplace(layers.begin(), layer);
}

void LayerManager::pop_layer(Layer* layer)
{
	auto i = std::find(layers.begin(), layers.end(), layer);

	if (i != layers.end())
	{
		layers.erase(i);
	}
}
