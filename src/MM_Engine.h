#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MM_Camera.h"
#include "MM_GameObject.h"
#include "MM_GameObjectManager.h"
#include "MM_UIManager.h"
#include "MM_Renderer.h"
#include "MM_TextManager.h"
#include "MM_ResourceManager.h"

#include "../res/string.h"
#include "MM_Utility.h"
#include "MM_Window.h"

#include "MM_Typedefs.h"

struct Engine
{
  GLFWwindow                        *window{NULL};
  Renderer                           renderer;
  Camera                             mainCamera;
  UIManager                          uiManager;
  GameObjectManager                  gameObjectManager;
  ResourceManager                    resourceManager;
  TextManager                       *textManager{NULL};

  u16			      width               { 640 };
  u16			      height              { 400 };
  string        title               { "Engine" };
  f64           openGLVersion       { 4.1 };
  u32           framerate           { 0 };
  f32           refreshRateInMS     { 60.0f };
  f32           fixedFrametime      { 0.0f };
  u8			      vsync               { 1 };
  u16           fps_info;

  bool          fullscreen          { false };
  bool          isRunning           { true };
  u32           currentFPS          { 0 };
  f64           deltaTime           { 0.0 };
  f64           timeSinceStart      { 0.0 };

  void init();
  void start();

  void gameLoop();
  void update();
  void draw();

  template <typename T>
  void addUI(T& ui);
  u32  addText(Text &text);
  Text& getText(u32 id);
  void updateText(u32 id, const char* new_string);
  void addGameObject(GameObject &gameobject);
};

template <typename T>
void Engine::addUI(T& ui)
{
  ui.id = uiManager.uiObjects.size();
  uiManager.uiObjects.emplace_back(std::make_unique<T>(ui));
}

