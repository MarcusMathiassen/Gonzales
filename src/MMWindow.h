#ifndef _MM_WINDOW_H
#define _MM_WINDOW_H

#include <glm/glm.hpp>

#define GLFW_DLL
#include <GLFW/glfw3.h>

struct MMWindow
{
  GLFWwindow *window;
  glm::vec4 clearColor{0.1f,0.1f,0.1f,1.0f};
};

#endif