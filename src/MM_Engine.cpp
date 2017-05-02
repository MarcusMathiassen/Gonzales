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

  ImGui_ImplGlfwGL3_Init(window, false);

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
		glfwPollEvents();
		const f64 timeStartFrame{ glfwGetTime() };
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
	glfwTerminate();
}

void Engine::gameLoop()
{
	f64 timeSpentSwapBuffer{ 0.0 };
	glfwSetTime(0.0);
	glfwMakeContextCurrent(window);

	while (isRunning)
	{
		const f64 timeStartFrame{ glfwGetTime() };
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		update();
		draw();

    char fpsinfo[20];
    sprintf(fpsinfo,"%dfps %fms",currentFPS, deltaTime);
    drawText(fpsinfo, -1.0, -1.0, mainCamera.aspectRatio);
		if (framerate > 0)
      limitFPS(framerate, timeStartFrame - timeSpentSwapBuffer);

		const f64 timeStartSwapBuffer{ glfwGetTime() };
		glfwSwapBuffers(window);
		timeSpentSwapBuffer = glfwGetTime() - timeStartSwapBuffer;

		deltaTime = (glfwGetTime() - timeStartFrame)*1000.0;
	}
	glfwMakeContextCurrent(NULL);
}

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImColor(10, 10, 10);


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
  textManager->drawAll();

  ImGui_ImplGlfwGL3_NewFrame();

  // 1. Show a simple window
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
  {
      static float f = 0.0f;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::ColorEdit3("clear color", (float*)&clear_color);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  // 2. Show another simple window, this time using an explicit Begin/End pair
  if (show_another_window)
  {
      ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello");
      ImGui::End();
  }

  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
  if (show_test_window)
  {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui::ShowTestWindow(&show_test_window);
  }

  ImGui::Render();
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

