#ifndef _MM_APP_H_
#define _MM_APP_H_

#include <thread>
#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <atomic>
#include "MM_Utility.h"

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

/* Declarations */
struct MMApp;
static void mmInit(MMApp &app);
static void mmStart(const MMApp &app);
static void mmInternalDrawLoop(const MMApp &app);
void draw();

static std::atomic<bool> isRunning{true};
static double timeSinceStart{0.0};

static uint32_t currentFPS{60};
static std::atomic<uint32_t> numFrames{0};
static std::atomic<double> deltaTime{16.666666f};

/* Definitions */
struct MMApp
{
  GLFWwindow *window{NULL};

  uint16_t width                    { MM_DEFAULT_APP_WIDTH };
  uint16_t height                   { MM_DEFAULT_APP_HEIGHT };

  std::string   title               { MM_DEFAULT_APP_TITLE }; // @TODO: change to const char*
  float         openGLVersion       { MM_DEFAULT_GL_VERSION };

  uint32_t      framerate           { 0 };
  float         refreshRateInMS     { 60.0f };
  float         fixedFrametime      { 0.0f };
  uint8_t       vsync               { MM_VSYNC_ON };

  bool          noClear             { MM_DEFAULT_APP_CLEAR };
};

static void mmInit(MMApp &app)
{
  if (app.framerate > 0)
    app.fixedFrametime = 1000.0f/app.framerate;

  glfwInit();

  const int gl_major = (int)std::floor(app.openGLVersion);
  const int gl_minor = (int)((app.openGLVersion - gl_major)*10.00001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  app.window = glfwCreateWindow(app.width, app.height, app.title.c_str(), NULL, NULL);
  glfwMakeContextCurrent(app.window);

  int count;
  const GLFWvidmode* modes = glfwGetVideoModes( glfwGetPrimaryMonitor(), &count);
  app.refreshRateInMS = 1000.0f/(modes->refreshRate);

  if (gl_major > 2)
    glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(1.0f, 0.3f, 0.3f, 1.0f);

  if (app.framerate == 0)
    glfwSwapInterval(app.vsync);
  else
    glfwSwapInterval(0);

#ifdef MM_DEBUG
  std::cout << glGetString(GL_VERSION) << '\n';
  std::cout << glGetString(GL_VENDOR) << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';
#endif
}

static void mmStart(const MMApp &app)
{
  glfwMakeContextCurrent(NULL);
  std::thread drawThread(mmInternalDrawLoop, app);

  timeSinceStart = glfwGetTime();
  while(!glfwWindowShouldClose(app.window) &&
        !(glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
  {
    glfwPollEvents();
    double timeStartFrame{ glfwGetTime() };
    if (timeStartFrame - timeSinceStart >= 1.0)
    {
      currentFPS = numFrames;
      printf("%dfps %0.03fms\n", currentFPS, (float)deltaTime);
      numFrames = 0;
      ++timeSinceStart;
    }
    MM_sleepForMS(app.refreshRateInMS);
  }

  isRunning = false;
  drawThread.join();

  glfwTerminate();
}

static void mmInternalDrawLoop(const MMApp &app)
{
  double timeSpentSwapBuffer{0.0};
  glfwSetTime(0.0);
  glfwMakeContextCurrent(app.window);
  while(isRunning)
  {
    double timeStartFrame{ glfwGetTime() };
    if (app.noClear)
      glClear(GL_COLOR_BUFFER_BIT);
    ++numFrames;
    draw();
    if (app.framerate > 0)
      MM_limitFPS(app.framerate, timeStartFrame-timeSpentSwapBuffer);

    double timeStartSwapBuffer{glfwGetTime()};
    glfwSwapBuffers(app.window);
    timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

    deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
  }
  glfwMakeContextCurrent(NULL);
}

#endif
