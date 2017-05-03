#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#define DEBUG

#include "MM_Camera.h"
#include "MM_GameObject.h"
#include "MM_GameObjectManager.h"
#include "MM_WindowManager.h"
#include "MM_Renderer.h"
#include "MM_TextManager.h"
#include "MM_ResourceManager.h"

#include "MM_Utility.h"
#include "MM_Window.h"

#include "MM_Typedefs.h"

struct Engine
{
  WindowManager                     *windowManager{NULL};
  Renderer                          *renderer{NULL};
  Camera                            *camera{NULL};
  GameObjectManager                 *gameObjectManager{NULL};
  ResourceManager                   *resourceManager{NULL};
  TextManager                       *textManager{NULL};

  bool          internal_settings_changed {true};
  u16			      width               { 640 };
  u16			      height              { 400 };
  string        title               { "Default Title" };
  u32           framerate           { 0 };
  f32           fixedFrametime      { 0.0f };
  u16           fps_info;

  bool          fullscreen          { false };
  bool          quit                { false };
  u32           currentFPS          { 0 };
  f64           deltaTime           { 0.0 };
  f64           timeSinceStart      { 0.0 };


  #ifdef DEBUG
  bool          show_engine_debug               {false};
  bool          show_windowManager_debug        {false};
  bool          show_renderer_debug             {false};
  bool          show_resourceManager_debug      {false};
  void          display_debug_imGui();
  #endif

  void init();
  void start();

  void gameLoop();
  void update();
  void draw();
  void internal_update();

  void addGameObject(const char* handle, GameObject &gameobject);
};
