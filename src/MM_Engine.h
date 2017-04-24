#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "MM_Camera.h"
#include "MM_GameObject.h"
#include "MM_GameObjectManager.h"
#include "MM_UIManager.h"

#include <string>
#include "MM_Utility.h"
#include "MM_Window.h"

#include "MM_Typedefs.h"

class Engine
{

public:

  GLFWwindow                        *window{nullptr};
  Camera                             mainCamera;
  UIManager                          uiManager;
  GameObjectManager					         gameObjectManager;

  u16			      width               { 640 };
  u16			      height              { 400 };
  std::string   title               { "Engine" };
  double        openGLVersion       { 4.1 };
  u32			      framerate           { 0 };
  float         refreshRateInMS     { 60.0f };
  float         fixedFrametime      { 0.0f };
  u8			      vsync               { 1 };

  bool        fullscreen            { false };
  bool        isRunning             { true };
  u32         currentFPS            { 0 };
  double      deltaTime             { 0.0 };
  double      timeSinceStart        { 0.0 };

  void init();
  void start();

  void gameLoop();
  void update();
  void draw();

  template<typename T>
  void addUI(T &ui);
  void addGameObject(GameObject &gameobject);

};

template<typename T>
void Engine::addUI(T &ui)
{
  ui.id = uiManager.uiObjects.size();
  uiManager.uiObjects.emplace_back(std::make_unique<T>(ui));
}

