#pragma once
#include <vector>
#include <memory>
#include <bitset>
#include <array>

#include "..\CherryI.h"

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID get_component_type_id()
{
	static ComponentID last_id = 0;
	return last_id++;
}

template <typename T>
inline ComponentID get_component_type_id() noexcept
{
	static ComponentID type_id = get_component_type_id();
	return type_id;
}

constexpr std::size_t max_components = 32;

using ComponentBitSet = std::bitset<max_components>;
using ComponentArray = std::array<Component*, max_components>;

class Component
{
public:
	Entity* entity;

	virtual void on_init(){}
	virtual void on_update_tick(TimeStep dt){}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) {}

	virtual ~Component() {}
};

class Entity
{
public:

	Entity()
	{}

	virtual void on_init() {}
	virtual void on_update_tick(TimeStep dt) {}
	virtual void on_draw_call(Window* render_window, Renderer* renderer) {}

	bool is_active() const { return active; }
	void destroy() { active = false; }

	template <typename T>
	bool has_component() const
	{
		return ComponentBitSet[get_component_type_id<T>()];
	}

	template <typename T, typename... TArgs>
	T& add_component(TArgs&&... args)
	{
		T* c(new T(std::forward<TArgs>(args)...));
		c->entity = this;
		std::unique_ptr<Component> uq_ptr{ c };
		components.emplace_back(std::move(uq_ptr));
		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;

		c->on_init();
		return *c;
	}

	template <typename T>
	T& get_component() const
	{
		auto ptr(component_array[get_component_type_id<T>()]);
		return *static_cast<T*>(ptr);
	}

private:

	friend class Manager;
	
	void update(TimeStep dt)
	{
		for (auto& component : components)
		{
			component->on_update_tick(dt);
		}

		on_update_tick(dt);
	}

	void draw(Window* render_window, Renderer* renderer)
	{
		for (auto& component : components)
		{
			component->on_draw_call(render_window, renderer);
		}
		on_draw_call(render_window, renderer);
	}


private:

	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray component_array;
	ComponentBitSet component_bitset;
};


class Manager
{

public:
	void update(TimeStep dt)
	{
		for (auto& entity : entities)
		{
			entity->update(dt);
		}
	}
	void draw(Window* render_window, Renderer* renderer)
	{
		for (auto& entity : entities)
		{
			entity->draw(render_window, renderer);
		}
	}

	// Erase every entity that is not active anymore (destroy() called)
	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& entity)
			{
				return !entity->is_active();
			}),
			std::end(entities));

	}
	template<typename EntityDerived=Entity>
	EntityDerived& add_entity()
	{
		EntityDerived* e = new EntityDerived();
		std::unique_ptr<EntityDerived> uq_ptr{ e };
		entities.emplace_back(std::move(uq_ptr));
		return *e;
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
};