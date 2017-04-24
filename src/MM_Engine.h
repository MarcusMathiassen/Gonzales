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
  GLFWwindow                        *window{NULL};
  Camera                             mainCamera;
  UIManager                          uiManager;
  GameObjectManager					         gameObjectManager;

  u16			      width               { 512 };
  u16			      height              { 512 };
  std::string   title               { "Engine" };
  double        openGLVersion       { 4.1 };
  u32			      framerate           { 0 };
  float         refreshRateInMS     { 60.0f };
  float         fixedFrametime      { 0.0f };
  u8			      vsync               { 1 };


  bool                 isRunning{ true };
  double               timeSinceStart{ 0.0 };
  u32                  currentFPS{ 0 };
  double               deltaTime{ 0.0 };

  void init();
  void start();

  void gameLoop();
  void update();
  void draw();

  void addUI(UI &ui);
  void addGameObject(GameObject &gameobject);
};
