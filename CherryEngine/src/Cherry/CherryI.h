#pragma once

#ifdef CHERRY_BUILD_DLL

#define CHERRY_API __declspec(dllexport)
#else
#define CHERRY_API __declspec(dllimport)

#endif

#include <iostream>

#include <GL/glew.h>
#include "OpenGL/Window.h"
#include "OpenGL/Devices.h"
