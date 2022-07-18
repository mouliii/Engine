#include "Window.h"

std::vector<Event> Window::events;

Window::Window()
	:
	width(800),
	height(600)
{
	init();
}

bool Window::window_should_close() const
{
	return glfwWindowShouldClose(this->gl_window);
}

void Window::poll_events() const
{
	glfwSwapBuffers(this->gl_window);
	glfwPollEvents();
}

void Window::wait_events() const
{
	glfwSwapBuffers(this->gl_window);
	glfwWaitEvents();
}

const std::vector<Event>& Window::get_events() const
{
	events.clear();
	poll_events();
	return events;
}

TimeStep Window::get_timestep()
{
	this->current_frame = (float)glfwGetTime();
	this->dt = current_frame - last_frame;
	this->last_frame = current_frame;
	return dt;
}

float Window::get_fps() const
{
	return 1.f / this->dt;
}

vec2i Window::get_size() const
{
	return { (int)width, (int)height };
}

vec2f Window::get_sizef() const
{
	return { (float)width, (float)height };
}

void Window::set_title(const std::string& title)
{
	glfwSetWindowTitle(this->gl_window, title.c_str());
}

GLFWwindow* Window::_get_glfw_window()
{
	return gl_window;
}

Window::~Window()
{
	glfwTerminate();

}

void Window::init()
{

	if (!glfwInit())
		throw("gl not inited");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->gl_window = glfwCreateWindow(width, height, "title", nullptr, nullptr);
	if (!this->gl_window)
	{
		glfwTerminate();
		throw("gl not inited");
	}
	

	glfwMakeContextCurrent(this->gl_window);

	if (glewInit() != GLEW_OK)
		throw "window init failure";


	// opengl blending options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set event functions for glfw to call
	glfwSetKeyCallback(this->gl_window, key_callback);
	glfwSetCursorPosCallback(this->gl_window, cursor_callback);
	glfwSetMouseButtonCallback(this->gl_window, mouse_button_callback);
	glfwSetScrollCallback(this->gl_window, scroll_callback);
	
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	Event e;
	e.keyboard_ev.key = key;
	e.keyboard_ev.action = action;
	e.keyboard_ev.scancode = scancode;
	e.keyboard_ev.mods = mods;
	e.type = EventType::KEYBOARD;
	events.push_back(e);

}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	Event e;
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	e.mouse_ev.x = (float)xpos;
	e.mouse_ev.y = h - (float)ypos;
	e.mouse_ev.button = -1;
	e.mouse_ev.action = -1;
	e.mouse_ev.mods = -1;
	e.type = EventType::MOUSE;

	events.push_back(e);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Event e;
	double x, y;
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	glfwGetCursorPos(window, &x, &y);
	e.mouse_ev.x = (float)x;
	e.mouse_ev.y = h - (float)y;
	e.mouse_ev.button = button;
	e.mouse_ev.action = action;
	e.mouse_ev.mods = mods;
	e.type = EventType::MOUSE;
	events.push_back(e);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Event e;
	e.type = EventType::SCROLL;

	e.scroll_ev.x_offset = xoffset;
	e.scroll_ev.y_offset = yoffset;

	events.push_back(e);

}
