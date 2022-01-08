#include "Devices.h"

bool KeyBoard::is_key_pressed(Key key, const Window* render_window)
{
	return glfwGetKey(render_window->gl_window, key) == GLFW_PRESS;
}

vec2f Mouse::get_mouse_pos(Window* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window->gl_window, &xpos, &ypos);

	return { (float)xpos, window->height - (float)ypos };
}

const std::array<bool, 5> Mouse::is_mouse_pressed(const Window* window)
{
	
	std::array<bool, 5> buttons;
	for (int i = 0; i < 5; i++)
	{
		buttons[i] = glfwGetMouseButton(window->gl_window, i);
	}
	return buttons;

	
}
