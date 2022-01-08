#pragma once

#include <iostream>

#include "GL/glew.h"


enum class DataType
{
	NONE = -1,
	FLOAT,
	UINT,
	INT
};

static GLenum get_opengl_datatype(DataType type)
{
	switch (type)
	{
	case DataType::NONE:
	{
		std::cout << "Infalid Datatype at: " << __FILE__ << " " << __LINE__ << std::endl;
		return -1;
	}
	case DataType::FLOAT:	return GL_FLOAT;
	case DataType::UINT:	return GL_UNSIGNED_INT;
	case DataType::INT:		return GL_INT;

	default:				break;
	}
	return -1;
}

static uint32_t get_datatype_size(DataType type)
{
	switch (type)
	{
	case DataType::NONE:
	{
		std::cout << "Infalid Datatype at: " << __FILE__ << " " << __LINE__ << std::endl;
		return -1;
	}
	case DataType::FLOAT:	return sizeof(float);
	case DataType::UINT:	return sizeof(uint32_t);
	case DataType::INT:		return sizeof(int);

	default:				break;
	}
	return -1;
}


struct Attribute
{
	Attribute(DataType type, uint32_t count, std::string name, bool normalized=false)
		:
		type(type), count(count), name(name), normalized(normalized)
	{}
	
	std::string name;
	uint32_t count;
	DataType type;
	bool normalized;
	uint32_t offset = 0;
};

