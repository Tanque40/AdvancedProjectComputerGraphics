#pragma once

#include <Glad/glad.h>

#define ASSERT(x) if((!(x))) __debugbreak();
#define GLCall(x) GLClearError(); \
		x; \
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall( const char* fucntion, const char* file, int line );