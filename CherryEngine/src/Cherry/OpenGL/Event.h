#pragma once
#include <vector>

enum class EventType
{
	KEYBOARD=1,
	MOUSE,
	SCROLL
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
};


typedef const std::vector<Event>& EventVector;