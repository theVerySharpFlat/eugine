#include "error.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

void GlClearError() {
  while (glGetError() != GL_NO_ERROR);
}

bool GLGetErrors(const char *function, const char *file, int line) {
  GLenum error = glGetError();
  bool errorOccured = false;
  while (error != GL_NO_ERROR) {
    errorOccured = true;
    std::string errorType;
    switch (error) {
    case GL_INVALID_ENUM:
      errorType = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      errorType = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      errorType = "GL_INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      errorType = "GL_STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      errorType = "GL_STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      errorType = "GL_OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      errorType = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    case GL_CONTEXT_LOST:
      errorType = "GL_CONTEXT_LOST";
      break;
    case GL_TABLE_TOO_LARGE:
      errorType = "GL_TABLE_TOO_LARGE";
      break;
    default:
      errorType = "UNKNOWN_ERROR";
      break;
    }
    std::cout << "[OpenGL Error" << std::hex << error << " (" << errorType
              << ")] in " << function << " at " << file << " on line "
              << std::dec << line << std::endl;
    error = glGetError();
  }
  return errorOccured;
}
