#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MM_Typedefs.h"

struct WindowManager
{
  GLFWwindow *window{NULL};

  s32           width                 { 640 };
  s32           height                { 400 };
  s32           viewport_width        { 400 };
  s32           viewport_height       { 400 };
  string        title                 { "Default title" };
  bool          fullscreen            { false };
  u16           monitor_refresh_rate  { 60 };
  u8            vsync                 { 1 };


  void createWindow(u32 width, u32 height, string title);

  void update();

  void initGLFW();
  void initGLEW();

  void display_debug_imGui();
};
