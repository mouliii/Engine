#pragma once
#include <iostream>
#include <vector>

#include "GL\glew.h"
class IndexBuffer
{
public:
	IndexBuffer();
	IndexBuffer(const void* data, uint32_t count);
	~IndexBuffer();
	void set_data(const void* data, uint32_t count);
	const uint32_t& get_count() const;
	void bind() const;
	void unbind() const;
	
private:
	uint32_t id;
	uint32_t count;
	
};

