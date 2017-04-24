#include "MM_Engine.h"

#include <thread>
#include <iostream>
#include <cmath>
#include <memory>

#include "MM_UIButton.h"

void Engine::init()
{
  if (framerate > 0) fixedFrametime = 1000.0f/framerate;

  glfwInit();

  const int gl_major = (int)std::floor(openGLVersion);
  const int gl_minor = (int)((openGLVersion - gl_major)*10.00001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  glfwMakeContextCurrent(window);

  int count;
  const GLFWvidmode* modes = glfwGetVideoModes( glfwGetPrimaryMonitor(), &count);
  refreshRateInMS = 1000.0f/(modes->refreshRate);

  if (gl_major > 2) glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initWindowCallbacks(window);

  if (framerate == 0) glfwSwapInterval(vsync);
  else                    glfwSwapInterval(0);

  std::cout << glGetString(GL_VERSION) << '\n';
  std::cout << glGetString(GL_VENDOR) << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  mmMainCamera = new Camera;
  mmMainCamera->aspectRatio = (float)width/(float)height;
  mmMainCamera->updatePerspective();
}


void Engine::start()
{
	glfwMakeContextCurrent(NULL);
	std::thread drawThread(&Engine::gameLoop, this);

	timeSinceStart = glfwGetTime();
	while (!glfwWindowShouldClose(window) &&
		!(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
	{
		glfwPollEvents();
		const double timeStartFrame{ glfwGetTime() };
		if (timeStartFrame - timeSinceStart >= 1.0)
		{
			currentFPS = (u32)(1000.0f / (float)deltaTime);
			++timeSinceStart;
		}
		sleepForMs(refreshRateInMS*0.5f); // update twice each screen refresh
	}
	isRunning = false;
	drawThread.join();
	glfwTerminate();
}

void Engine::update()
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  mainCamera.aspectRatio = (float)width / (float)height;

  gameObjectManager.update();
	uiManager.update(mainCamera);
}

void Engine::draw()
{
  gameObjectManager.draw();
  uiManager.draw();
}

void Engine::gameLoop()
{
	double timeSpentSwapBuffer{ 0.0 };
	glfwSetTime(0.0);
	glfwMakeContextCurrent(window);

	while (isRunning)
	{
		const double timeStartFrame{ glfwGetTime() };
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// FRAME START

		update();
		draw();

		// FRAME END
		if (framerate > 0)
      limitFPS(framerate, timeStartFrame - timeSpentSwapBuffer);

		const double timeStartSwapBuffer{ glfwGetTime() };
		glfwSwapBuffers(window);
		timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

		deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
	}
	glfwMakeContextCurrent(NULL);
}

void Engine::addUI(UI &ui)
{
  ui.id = uiManager.uiObjects.size();
  uiManager.uiObjects.emplace_back(std::make_unique<UI>(ui));
}

void Engine::addGameObject(GameObject &gameobject)
{
	gameobject.id = gameObjectManager.gameObjects.size();
	gameObjectManager.gameObjects.emplace_back(std::make_unique<GameObject>(gameobject));
}
