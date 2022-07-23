#pragma once


#include "../Application.h"
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_impl_glfw.h"
#include "../../../imgui/imgui_impl_opengl3.h"


class ImGuiLayer : public Layer  
{

public:
	void on_init() override;
	void on_update(TimeStep) override;
	void on_loop_end() override;
	void set_render_data();

private:

	void entity_tree();

	bool construct_shape(Entity entity);
	bool delete_item_on_right_click();

	void shape_component_button(Entity entity);
	void display_component_button(Entity entity);
	void collision_component_button(Entity entity);

	void create_collision_component(Entity entity);
	void create_display_component(Entity entity);
	void create_shape_component(Entity entity);

	void TestFBO();
};

