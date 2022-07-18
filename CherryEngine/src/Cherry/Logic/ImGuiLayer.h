#pragma once


#include "CrEngine.h"
#include "../../../imgui/imgui.h"
#include "../../../imgui/imgui_impl_glfw.h"
#include "../../../imgui/imgui_impl_opengl3.h"


class ImGuiLayer : public SandBoxBase  
{

public:
	void on_init() override;
	void on_game_tick(TimeStep) override;
	void on_draw_call(Window*, Renderer*) override;
	void on_loop_end() override;
	void set_render_data();

private:

	bool construct_shape(Entity* e);
	bool delete_item_on_right_click();
};

