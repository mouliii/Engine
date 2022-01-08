#pragma once

#include "Window.h"

typedef int Key;

class KeyBoard
{
public:
	static bool is_key_pressed(Key key, const Window* render_window);

};


class Mouse
{
public:

	static vec2f get_mouse_pos(Window* window);
	static const std::array<bool, 5> is_mouse_pressed(const Window* window);

};