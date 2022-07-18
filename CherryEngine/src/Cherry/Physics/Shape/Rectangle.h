#pragma once
#include "Polygon.h"

class Rectangle : public Polygon
{
public:
	Rectangle();
	Rectangle(vec2f position, vec2f size);
	vec2f get_size();
	
};


