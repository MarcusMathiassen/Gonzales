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

  const u8 gl_major = (u8)std::floor(openGLVersion);
  const u8 gl_minor = (u8)((openGLVersion - gl_major)*10.0001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  glfwMakeContextCurrent(window);

  s32 count;
  const GLFWvidmode* modes = glfwGetVideoModes( glfwGetPrimaryMonitor(), &count);
  refreshRateInMS = 1000.0f/(modes->refreshRate);

  if (gl_major > 2) glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  initWindowCallbacks(window);

  if (framerate == 0) glfwSwapInterval(vsync);
  else                glfwSwapInterval(0);

  std::cout << glGetString(GL_VERSION)  << '\n';
  std::cout << glGetString(GL_VENDOR)   << '\n';
  std::cout << glGetString(GL_RENDERER) << '\n';

  s32 width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  this->width  = width;
  this->height = height;

  mainCamera.aspectRatio = (f32)width / height;
  mainCamera.updatePerspective();
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
		const f64 timeStartFrame{ glfwGetTime() };
		if (timeStartFrame - timeSinceStart >= 1.0)
		{
			currentFPS = (u32)(1000.0f / (f32)deltaTime);
			++timeSinceStart;
		}
		sleepForMs(refreshRateInMS);
	}
	isRunning = false;
	drawThread.join();
	glfwTerminate();
}

void Engine::update()
{
  // @Hack: please for the love of god fix this
  s32 width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  this->width  = width;
  this->height = height;

  mainCamera.aspectRatio = (f32)width / height;
  mainCamera.updatePerspective();

  gameObjectManager.update();
	uiManager.update(mainCamera);
}

void Engine::draw()
{
  gameObjectManager.draw(mainCamera);
  uiManager.draw();
}

void Engine::gameLoop()
{
	f64 timeSpentSwapBuffer{ 0.0 };
	glfwSetTime(0.0);
	glfwMakeContextCurrent(window);

	while (isRunning)
	{
		const f64 timeStartFrame{ glfwGetTime() };
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		update();
		draw();

    drawText(std::to_string(currentFPS)+"fps "+std::to_string(deltaTime)+"ms", -1.0, -1.0, mainCamera.aspectRatio);

		if (framerate > 0)
      limitFPS(framerate, timeStartFrame - timeSpentSwapBuffer);

		const f64 timeStartSwapBuffer{ glfwGetTime() };
		glfwSwapBuffers(window);
		timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

		deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
	}
	glfwMakeContextCurrent(NULL);
}

void Engine::addGameObject(GameObject &gameobject)
{
	gameobject.id = (u32)gameObjectManager.gameObjects.size();
	gameObjectManager.gameObjects.emplace_back(std::make_unique<GameObject>(gameobject));
}

