#include "MM_Engine.h"
#include "MM_Text.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <stdio.h>
#include <string.h>
#include <thread>
#include <math.h>

void Engine::init()
{
  if (framerate > 0) fixedFrametime = 1000.0f/framerate;

  // @Cleanup: this is window initialization. Do it somewhere else.
  glfwInit();

  const u8 gl_major = (u8)std::floor(openGLVersion);
  const u8 gl_minor = (u8)((openGLVersion - gl_major) * 10.0001f);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);

  if (gl_major > 2)
  {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  window = glfwCreateWindow(width,
                            height,
                            title.c_str(),
                            fullscreen ? glfwGetPrimaryMonitor() : NULL,
                            NULL);

  glfwMakeContextCurrent(window);

  s32 count;
  const GLFWvidmode* modes = glfwGetVideoModes( glfwGetPrimaryMonitor(), &count);
  refreshRateInMS = 1000.0f/(modes->refreshRate);

  if (gl_major > 2) glewExperimental = GL_TRUE;

  glewInit();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  ImGui_ImplGlfwGL3_Init(window, true);

  initWindowCallbacks(window);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (framerate == 0) glfwSwapInterval(vsync);
  else                glfwSwapInterval(0);

  // @Cleanup: move this somewhere else
  printf("%s\n", glGetString(GL_VERSION));
  printf("%s\n", glGetString(GL_VENDOR));
  printf("%s\n", glGetString(GL_RENDERER));

  s32 width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  this->width  = width;
  this->height = height;

  mainCamera.aspectRatio = (f32)width / height;
  mainCamera.updatePerspective();

  textManager = new TextManager;
}

void Engine::start()
{
	glfwMakeContextCurrent(NULL);
	std::thread drawThread(&Engine::gameLoop, this);

  Text text;
  text.color = glm::vec4(1, 0, 0, 1);
  text.pos.x = -1;
  text.pos.y = -0.9375;
  u32 id = addText(text);

	timeSinceStart = glfwGetTime();
	while (!glfwWindowShouldClose(window) &&
		!(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
	{
    const f64 timeStartFrame{ glfwGetTime() };
		glfwPollEvents();
		if (timeStartFrame - timeSinceStart >= 1.0)
    {
      currentFPS = (u32)(1000.0f / (f32)deltaTime);
    	++timeSinceStart;
    }

    char fpsinfo[20];
    sprintf(fpsinfo,"%dfps %fms", currentFPS, deltaTime);
    Text &text = getText(id);
    text.str = fpsinfo;

    // @Cleanup: do we need to sleep?
		sleepForMs(refreshRateInMS);
	}

  // @Cleanup: place this somewhere else
	isRunning = false;
	drawThread.join();
  ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

ImVec4 clear_color = ImColor(105,183,121);
void Engine::gameLoop()
{
	f64 timeSpentSwapBuffer{ 0.0 };
	glfwSetTime(0.0);
	glfwMakeContextCurrent(window);

	while (isRunning)
	{
		const f64 timeStartFrame{ glfwGetTime() };
    ImGui_ImplGlfwGL3_NewFrame();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    update();
    draw();

    char fpsinfo[20];
    sprintf(fpsinfo,"%dfps %fms",currentFPS, deltaTime);
    drawText(fpsinfo, -1.0, -1.0, mainCamera.aspectRatio);
		if (framerate > 0)
      limitFPS(framerate, timeStartFrame - timeSpentSwapBuffer);

    ImGui::Render();
		const f64 timeStartSwapBuffer{ glfwGetTime() };
		glfwSwapBuffers(window);
		timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

		deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
	}
	glfwMakeContextCurrent(NULL);
}

void Engine::internal_update()
{
  // @Cleanup: put this somewhere else
  s32 width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  this->width  = width;
  this->height = height;
  mainCamera.aspectRatio = (f32)width / height;
  mainCamera.updatePerspective();

  if (framerate == 0) glfwSwapInterval(vsync);
  else                glfwSwapInterval(0);

  glClearColor(clear_color.x,clear_color.y,clear_color.z,1.0);
}

void Engine::update()
{
  if (internal_settings_changed) internal_update();

  gameObjectManager.update();
  uiManager.update(mainCamera);
}
void Engine::draw()
{
  gameObjectManager.draw(mainCamera);
  uiManager.draw();
  textManager->drawAll();

  ImGui::Begin("Info");
  ImGui::Checkbox("vsync", (bool*)&vsync);
  ImGui::SameLine();
  ImGui::Text("%dfps %fms",currentFPS, deltaTime);
  ImGui::ColorEdit3("clear color", (float*)&clear_color);
  ImGui::End();
}

u32 Engine::addText(Text &text)
{
  const u32 id{ (u32)textManager->text_buffer.size() };
  text.id = id;
  textManager->text_buffer.emplace_back(text);
  return id;
}

void Engine::addGameObject(const char* handle, GameObject &gameobject)
{
	gameobject.id = (u32)gameObjectManager.gameObjects.size();
	gameObjectManager.gameObjects.emplace_back(std::make_unique<GameObject>(gameobject));
  // resourceManager.addGameObject(handle, std::make_unique<GameObject>(gameobject));
}

Text& Engine::getText(u32 id)
{
  for (auto &text: textManager->text_buffer)
    if (id == text.id) return text;
}

void Engine::updateText(u32 id, const char* new_string)
{
  for (auto &text: textManager->text_buffer)
  {
    if (id == text.id) text.str = new_string;
  }
}



