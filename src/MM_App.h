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
#include "MM_Window.h"
#include "MM_GameObject.h"
#include "MM_Camera.h"
//#include "MM_GameObjectManager.h"

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

/* Declarations */
struct MMApp;

void draw(); // users drawloop
static void mmInit(MMApp &app);
static void mmStart(const MMApp &app);
static void mmInternalDrawLoop(const MMApp &app);

/* Definitions */
struct MMApp
{
  GLFWwindow           *window{NULL};
  //MMGameObjectManager  *mmGameObjectManager{NULL};

  uint16_t      width               { MM_DEFAULT_APP_WIDTH };
  uint16_t      height              { MM_DEFAULT_APP_HEIGHT };
  std::string   title               { MM_DEFAULT_APP_TITLE }; // @TODO: change to const char*
  float         openGLVersion       { MM_DEFAULT_GL_VERSION };
  uint32_t      framerate           { 0 };
  float         refreshRateInMS     { 60.0f };
  float         fixedFrametime      { 0.0f };
  uint8_t       vsync               { MM_VSYNC_ON };
  bool          noClear             { MM_DEFAULT_APP_NO_CLEAR };

  static std::atomic<bool>    isRunning;
  static double               timeSinceStart;
  static uint32_t             currentFPS;
  static std::atomic<double>  deltaTime;
};

uint32_t            MMApp::currentFPS{60};
double              MMApp::timeSinceStart{0.0};
std::atomic<bool>   MMApp::isRunning{true};
std::atomic<double> MMApp::deltaTime{16.666666f};

static void mmInit(MMApp &app)
{
  if (app.framerate > 0) app.fixedFrametime = 1000.0f/app.framerate;

  glfwInit();

  const int gl_major = (int)std::floor(app.openGLVersion);
  const int gl_minor = (int)((app.openGLVersion - gl_major)*10.00001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
  glfwWindowHint(GL_RED_BITS, 8);
  glfwWindowHint(GL_GREEN_BITS, 8);
  glfwWindowHint(GL_BLUE_BITS, 8);
  glfwWindowHint(GL_ALPHA_BITS, 8);
  glfwWindowHint(GL_BUFFER_SIZE, 32);
  glfwWindowHint(GL_DEPTH_BITS, 16);
  glfwWindowHint(GL_DOUBLEBUFFER, 1);
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

  if (gl_major > 2) glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initWindowCallbacks(app.window);

  if (app.framerate == 0) glfwSwapInterval(app.vsync);
  else                    glfwSwapInterval(0);

  std::cout << glGetString(GL_VERSION) << '\n';
  std::cout << glGetString(GL_VENDOR) << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';

  int width, height;
  glfwGetFramebufferSize(app.window, &width, &height);
  glViewport(0, 0, width, height);

  mmMainCamera = new MMCamera;
  mmMainCamera->aspectRatio = (float)width/(float)height;
  mmMainCamera->updatePerspective();
}

static void mmStart(const MMApp &app)
{
  glfwMakeContextCurrent(NULL);
  std::thread drawThread(mmInternalDrawLoop, app);

  app.timeSinceStart = glfwGetTime();
  while(!glfwWindowShouldClose(app.window) &&
        !(glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
  {
    glfwPollEvents();
    const double timeStartFrame{ glfwGetTime() };
    if (timeStartFrame - app.timeSinceStart >= 1.0)
    {
      app.currentFPS = (uint32_t)(1000.0f/(float)app.deltaTime);
      ++app.timeSinceStart;
    }
    mmSleepForMS(app.refreshRateInMS*0.5); // update twice each screen refresh
  }
  app.isRunning = false;
  drawThread.join();
  glfwTerminate();
}


// @Cleanup: this is horrible. we're updating the viewport each frame.
static void internalChecks(GLFWwindow *window)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

static void mmInternalDrawLoop(const MMApp &app)
{
  double timeSpentSwapBuffer{0.0};
  glfwSetTime(0.0);
  glfwMakeContextCurrent(app.window);
  while(app.isRunning)
  {
    const double timeStartFrame{ glfwGetTime() };
    if (!app.noClear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    internalChecks(app.window);

    draw(); // users draw is called

    if (app.framerate > 0) mmLimitFPS(app.framerate, timeStartFrame-timeSpentSwapBuffer);

    const double timeStartSwapBuffer{glfwGetTime()};
    glfwSwapBuffers(app.window);
    timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

    app.deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
  }
  glfwMakeContextCurrent(NULL);
}

#endif
