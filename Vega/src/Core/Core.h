#pragma once

#include <iostream>

#include "Debug/ConsoleLog.h"

#ifdef DEBUG
	#define CORE_ASSERT(x, ...) { if (!(x)) { LOGE(__VA_ARGS__); __debugbreak; } }
#else
	#define CORE_ASSERT(x, ...) 
#endif