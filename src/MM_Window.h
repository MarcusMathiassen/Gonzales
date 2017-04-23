#ifndef _MM_WINDOW_H_
#define _MM_WINDOW_H_

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "MM_Camera.h"

static void window_iconify_callback(GLFWwindow* window, int iconified);
static void window_focus_callback(GLFWwindow* window, int focused);
static void drop_callback(GLFWwindow* window, int count, const char** paths);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void initWindowCallbacks(GLFWwindow* window)
{
  // glfwSetWindowAspectRatio(window, 1, 1);
  glfwSetWindowIconifyCallback(window, window_iconify_callback);
  glfwSetWindowFocusCallback(window, window_focus_callback);
  glfwSetDropCallback(window, drop_callback);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

static void window_iconify_callback(GLFWwindow* window, int iconified)
{
  if (iconified)
  {
    std::cout << "The window was iconified" << std::endl;
  }
  else
  {
    std::cout << "The window was restored" << std::endl;
  }
}

static void window_focus_callback(GLFWwindow* window, int focused)
{
  if (focused)
  {
    std::cout << "The window gained input focus" << '\n';
  }
  else
  {
    std::cout << "The window lost input focus" << '\n';
  }
}

static void drop_callback(GLFWwindow* window, int count, const char** paths)
{
  std::cout << "Drag dropped: " << (*paths) << '\n';
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  mmMainCamera->aspectRatio = (float)width / (float)height;
  mmMainCamera->updatePerspective();
  std::cout << "SCREEN RESIZE: " << width << "x" << height << '\n';
}

#endif
