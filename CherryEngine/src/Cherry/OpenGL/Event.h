#pragma once
#include <vector>

enum class EventType
{
	KEYBOARD=1,
	MOUSE,
	SCROLL,
	WINDOW_RESIZE,
	FRAMEBUFFER_RESIZE
};

struct KeyboardEvent
{
	int key;
	int action;
	int scancode;
	int mods;
};

struct MouseEvent
{
	float x;
	float y;
	int button;
	int action;
	int mods;
};

struct WindowResizeEvent
{
	int w;
	int h;
};

struct FrameBufferResizeEvent
{
	int w;
	int h;
};

struct ScrollEvent
{
	float x_offset;
	// y_offset is the normal mouse wheel offset
	float y_offset;
};

struct Event
{
	EventType type;
	KeyboardEvent keyboard_ev;
	MouseEvent mouse_ev;
	ScrollEvent scroll_ev;
	WindowResizeEvent window_res_ev;
	FrameBufferResizeEvent frame_buf_res_ev;
};


typedef const std::vector<Event>& EventVector;