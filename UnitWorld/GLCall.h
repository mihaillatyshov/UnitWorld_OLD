#pragma once

#include <iostream>
#include <GL/glew.h>
#include <string>
#include "Debug/ConsoleLog.h"

#define DEBUG

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		LOGE("Opengl Error: ", error, (std::string(function) + " " + std::string(file) + " " + std::to_string(line)).c_str());
		return false;
	}
	return true;
}

#ifdef DEBUG
#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCall(x) GLClearError(); \
	x; \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));
#else
#define GLCall(x) x;
#endif