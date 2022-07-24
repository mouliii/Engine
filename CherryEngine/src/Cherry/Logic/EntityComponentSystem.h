#pragma once
#include <vector>
#include <memory>
#include <bitset>
#include <array>
#include <queue>
#include <set>

#include "..\CherryI.h"




using Entity = std::uint32_t;
constexpr std::uint32_t MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;

constexpr std::uint8_t MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;


class EntityManager
{
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
		{
			available_entries.push(entity);
		}
	}

	Entity create_entity(std::string name="entity")
	{

		Entity id = available_entries.front();
		entity_name_map[id] = name;
		available_entries.pop();

		entity_count++;

		return id;

	}

	void destroy_entity(Entity entity)
	{
		signatures[entity].reset();
		entity_name_map.erase(entity);
		available_entries.push(entity);
		entity_count--;
	}

	void set_signature(Entity entity, Signature signature)
	{
		signatures[entity] = signature;
	}

	Signature get_signature(Entity entity)
	{
		return signatures[entity];
	}

	const std::string& get_entity_name(Entity entity)
	{
		return entity_name_map[entity];
	}


private:

	std::queue<Entity> available_entries;
	std::array<Signature, MAX_ENTITIES> signatures;

	std::unordered_map<Entity, std::string> entity_name_map;

	uint32_t entity_count = 0;

	
};


class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void entity_destroyed(Entity entity) = 0;
};


template <typename T>
class ComponentArray : public IComponentArray
{
public:

	void insert_data(Entity entity, T component)
	{
		entity_index_map[entity] = size;
		index_entity_map[size] = entity;
		component_array[size] = component;
		size++;
	}

	void remove_data(Entity entity)
	{

		size_t index_of_removed_entity = entity_index_map[entity];
		size_t last_element = size - 1;
		component_array[index_of_removed_entity] = component_array[last_element];

		Entity entity_of_last_element = index_entity_map[last_element];
		entity_index_map[entity_of_last_element] = index_of_removed_entity;
		index_entity_map[index_of_removed_entity] = entity_of_last_element;


		entity_index_map.erase(entity);
		index_entity_map.erase(last_element);
		
		size--;

	}

	T& get_data(Entity entity)
	{
		return component_array[entity_index_map[entity]];
	}

	void entity_destroyed(Entity entity) override
	{
		if (entity_index_map.find(entity) != entity_index_map.end())
		{
			remove_data(entity);
		}
	}




private:

	std::array<T, MAX_ENTITIES> component_array;

	std::unordered_map<Entity, size_t> entity_index_map;

	std::unordered_map<size_t, Entity> index_entity_map;

	uint32_t size;

};

class ComponentManager
{
public:

	template <typename T>
	void register_component()
	{
		const char* type_name = typeid(T).name();

		component_types.insert({ type_name, next_component_type });
		component_arrays.insert({ type_name, std::make_shared<ComponentArray<T>>() });

		next_component_type++;
	}

	template <typename T>
	ComponentType get_component_type()
	{
		const char* type_name = typeid(T).name();
		return component_types[type_name];
	}

	template<typename T>
	void add_component(Entity entity, T component)
	{
		get_component_array<T>()->insert_data(entity, component);
	}

	template <typename T>
	void remove_component(Entity entity)
	{
		get_component_array<T>()->remove_data(entity);
	}

	template <typename T>
	T& get_component(Entity entity)
	{
		return get_component_array<T>()->get_data(entity);
	}

	void entity_destroyed(Entity entity)
	{
		for (auto const& pair : component_arrays)
		{
			auto const& component = pair.second;
			component->entity_destroyed(entity);
		}
	}

	std::unordered_map<const char*, std::shared_ptr<IComponentArray>>& get_component_arrays()
	{
		return component_arrays;
	}


private:

	std::unordered_map<const char*, ComponentType> component_types;

	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays;

	ComponentType next_component_type;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> get_component_array() // helper funktio jolla saa kästättyä ComponentArray:n 
	{
		const char* type_name = typeid(T).name();

		return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type_name]);
	}
};

class System
{
public:
	std::set<Entity> entities;

};

class SystemManager
{
public:

	template<typename T>
	std::shared_ptr<T> register_system()
	{
		const char* type_name = typeid(T).name();

		auto system = std::make_shared<T>();

		systems.insert({ type_name, system });
		return system;

	}

	template <typename T>
	void set_signature(Signature signature)
	{
		const char* type_name = typeid(T).name();
		
		signatures.insert({ type_name, signature });

	}

	void entity_destroyed(Entity entity)
	{
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;
			system->entities.erase(entity);
		}
	}

	void entity_signature_changed(Entity entity, Signature entity_signature)
	{
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& system_signature = signatures[type];

			if ((entity_signature & system_signature) == system_signature)
			{
				system->entities.insert(entity);
			}

			else
			{
				system->entities.erase(entity);
			}
		}
	}

	std::unordered_map<const char*, std::shared_ptr<System>>& get_systems()
	{
		return systems;
	}

	template <typename T>
	std::shared_ptr<System> get_system()
	{
		const char* type_name = typeid(T).name();

		return systems[type_name];

	}

private:

	std::unordered_map<const char*, Signature> signatures;
	std::unordered_map<const char*, std::shared_ptr<System>> systems;

};


class ECSManager
{
public:
	ECSManager()
	{
		component_manager	= std::make_unique<ComponentManager>();
		entity_manager		= std::make_unique<EntityManager>();
		system_manager		= std::make_unique<SystemManager>();
	}

	// entity

	Entity create_entity(std::string name="entity")
	{
		Entity entity = entity_manager->create_entity(name);
		auto signature = entity_manager->get_signature(entity);
	
		entity_manager->set_signature(entity, signature);
		system_manager->entity_signature_changed(entity, signature);
		return entity;
	}

	void destroy_entity(Entity entity)
	{
		entity_manager->destroy_entity(entity);
		component_manager->entity_destroyed(entity);
		system_manager->entity_destroyed(entity);
	}

	const std::string& get_entity_name(Entity entity)
	{
		return entity_manager->get_entity_name(entity);
	}

	// component

	template <typename T>
	void register_component()
	{
		component_manager->register_component<T>();
	}

	template <typename T>
	void add_component(Entity entity, T component)
	{
		component_manager->add_component<T>(entity, component);

		auto signature = entity_manager->get_signature(entity);
		signature.set(component_manager->get_component_type<T>(), true);
		entity_manager->set_signature(entity, signature);
		system_manager->entity_signature_changed(entity, signature);
	}


	template <typename T>
	bool has_component(Entity entity)
	{
		auto signature = entity_manager->get_signature(entity);
		
		return (signature[component_manager->get_component_type<T>()]);
	}

	template <typename T>
	void remove_component(Entity entity)
	{
		component_manager->remove_component<T>(entity);

		auto signature = entity_manager->get_signature(entity);
		signature.set(component_manager->get_component_type<T>(), false);
		entity_manager->set_signature(entity, signature);
		system_manager->entity_signature_changed(entity, signature);
	}

	template <typename T>
	T& get_component(Entity entity)
	{
		return component_manager->get_component<T>(entity);
	}

	template <typename T>
	ComponentType get_component_type()
	{
		return component_manager->get_component_type<T>();
	}

	template <typename T>
	const char* get_component_name()
	{
		return typeid(T).name();
	}


	// system

	template<typename T>
	std::shared_ptr<T> register_system()
	{
		return system_manager->register_system<T>();
	}

	template<typename T>
	void set_system_signature(Signature signature)
	{
		system_manager->set_signature<T>(signature);
	}

	std::unique_ptr<SystemManager>& get_system_manager()
	{
		return system_manager;
	}

	std::unique_ptr<ComponentManager>& get_component_manager()
	{
		return component_manager;
	}

	std::unique_ptr<EntityManager>& get_entity_manager()
	{
		return entity_manager;
	}

private:
	std::unique_ptr<ComponentManager>	component_manager;
	std::unique_ptr<EntityManager>		entity_manager;
	std::unique_ptr<SystemManager>		system_manager;
};