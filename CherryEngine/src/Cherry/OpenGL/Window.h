#pragma once
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Event.h"
#include "Rendering/Renderer.h"
#include "../Maths/Vecs.h"

typedef const float& TimeStep;

class Window
{
public:
	
	Window();
	bool window_should_close() const;

	void poll_events() const;
	void wait_events() const;
	std::vector<Event>& get_events() const;
	
	// Get time between frames
	TimeStep get_timestep();
	float get_fps() const;

	vec2i get_size()  const;
	vec2f get_sizef() const;

	void set_title(const std::string& title);
	void set_size(uint32_t w, uint32_t h);

	GLFWwindow* _get_glfw_window();

	~Window();

	
private:

	void init();
	
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
private:

	GLFWwindow* gl_window;
	static std::vector<Event> events;

	uint32_t width, height;

	float dt, current_frame;
	float last_frame = 0;

	friend class KeyBoard;
	friend class Mouse;
};

