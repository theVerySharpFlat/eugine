#ifndef EUGINE_PLATFORM_OPENGL_WRAPPER_ERROR_H
#define EUGINE_PLATFORM_OPENGL_WRAPPER_ERROR_H

#include "eugine/core/core.h"
#include "glad/glad.h"

void GlClearError();

bool GLGetErrors(const char* function, const char* file, int line);

#define GLCall(x) GlClearError(); x; EG_ASSERT(!GLGetErrors(#x, __FILE__, __LINE__), "OpenGL call failed!");
#endif
