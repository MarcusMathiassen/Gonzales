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
struct MM_App;
static void MM_initApp(MM_App &app);
static void MM_startApp(const MM_App &app);
static void drawLoop(MM_App &app);
void draw();

static std::atomic<bool> isRunning{true};
static double timeSinceStart{0.0};

static uint32_t currentFPS{60};
static std::atomic<uint32_t> numFrames{0};
static std::atomic<float> deltaTime{16.666666f};

/* Definitions */
struct MM_App
{
  GLFWwindow *window{NULL};
  uint16_t width{512}, height{512};
  std::string title{"Default title"};
  float glVersion{2.1f};

  uint32_t fixedFramerate{60};
  float refreshRateInMS{60.0f};
  float fixedFrametime{1000.0f/fixedFramerate};
};

static void MM_initApp(MM_App &app)
{
  app.fixedFrametime = 1000.0f/app.fixedFramerate;

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

  int count;
  const GLFWvidmode* modes = glfwGetVideoModes( glfwGetPrimaryMonitor(), &count);
  app.refreshRateInMS = 1000.0f/(modes->refreshRate*2);

  if (gl_major > 2)
    glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(1.0f, 0.3f, 0.3f, 1.0f);

  glfwSwapInterval(0);

#ifdef MM_DEBUG
  std::cout << glGetString(GL_VERSION) << '\n';
  std::cout << glGetString(GL_VENDOR) << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';
#endif
}

static void MM_startApp(const MM_App &app)
{
  glfwMakeContextCurrent(NULL);
  std::thread drawThread = std::thread(drawLoop, app);

  timeSinceStart = glfwGetTime();
  while(!glfwWindowShouldClose(app.window) &&
        !glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

static void drawLoop(MM_App &app)
{
  glfwMakeContextCurrent(app.window);
  while(isRunning)
  {
    double timeStartFrame{ glfwGetTime() };
    ++numFrames;
    draw();
    MM_limitFPS(app.fixedFramerate, timeStartFrame);
    deltaTime = (float)((glfwGetTime() - timeStartFrame)*1000.0f);
    glfwSwapBuffers(app.window);
  }
  glfwMakeContextCurrent(NULL);
}

#endif
