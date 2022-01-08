#pragma once
#include <vector>

enum class EventType
{
	KEYBOARD=1,
	MOUSE
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

struct Event
{
	EventType type;
	KeyboardEvent keyboard_ev;
	MouseEvent mouse_ev;
};


typedef const std::vector<Event>& EventVector;