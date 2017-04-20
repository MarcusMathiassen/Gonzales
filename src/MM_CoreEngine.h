#ifndef _MM_COREENGINE_H_
#define _MM_COREENGINE_H_

#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

struct View
{
  uint16_t width{512}, height{512};
};

struct MM_CoreEngine
{
  GLFWwindow *window{NULL};
  View view;
  std::string title;
  float openGLVersion{2.1};
};
static MM_CoreEngine *app;

static GLFWwindow* MM_newWindow(int width, int height, std::string title, float openGLVersion)
{
  app = new MM_CoreEngine();
  app->view.width = width;
  app->view.height = height;
  app->title = title;
  app->openGLVersion = openGLVersion;

  glfwInit();

  const int gl_major = std::floor(openGLVersion);
  const int gl_minor = (openGLVersion - gl_major)*10.00001;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  app->window = window;
  glfwMakeContextCurrent(window);

  if (gl_major > 2)
    glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

  glfwSwapInterval(1);

  return window;
}

#endif
