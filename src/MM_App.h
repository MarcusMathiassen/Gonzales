#ifndef _MM_APP_H_
#define _MM_APP_H_

#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

struct MM_App
{
  GLFWwindow *window{NULL};
  uint16_t width{512}, height{512};
  std::string title{"Default title"};
  float glVersion{2.1f};
  float fixedFrameTime{16.666666f};
  float deltaTime{fixedFrameTime};

  bool hasRan{false};
  bool running();
  void shutdown();
};

static void MM_initApp(MM_App &app)
{
  glfwInit();

  const int gl_major = (int)std::floor(app.glVersion);
  const int gl_minor = (int)((app.glVersion - gl_major)*10.00001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  app.window = glfwCreateWindow(app.width, app.height, app.title.c_str(), NULL, NULL);
  glfwMakeContextCurrent(app.window);

  if (gl_major > 2)
    glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

  glfwSwapInterval(0);

#ifdef MM_DEBUG
  std::cout << glGetString(GL_VERSION) << '\n';
  std::cout << glGetString(GL_VENDOR) << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';
#endif
}

#endif
