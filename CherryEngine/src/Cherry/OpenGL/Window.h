#pragma once
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Event.h"
#include "Rendering/Renderer.h"

typedef const float& TimeStep;

class Window
{
public:
	
	Window();
	bool window_should_close() const;
	
	void wait_events() const;
	const std::vector<Event>& get_events() const;

	// Get time between frames
	TimeStep get_timestep();
	float get_fps();

	~Window();

	
private:

	void init();
	void poll_events() const;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

private:

	GLFWwindow* gl_window;
	static std::vector<Event> events;

	uint32_t width, height;

	float dt, current_frame;
	float last_frame = 0;

	friend class KeyBoard;
	friend class Mouse;
};

