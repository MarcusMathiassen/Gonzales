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
  windowManager = new WindowManager;
  windowManager->createWindow(width,height,title);

  ImGui_ImplGlfwGL3_Init(windowManager->window, true);
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("./res/DroidSans.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  auto& style = ImGui::GetStyle();
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.90f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.80f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.90f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.90f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.3f, 0.90f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1f, 0.1f, 0.1f, 0.90f);

  resourceManager = new ResourceManager;
  renderer = new Renderer;
  textManager = new TextManager;
  camera = new Camera;
  gameObjectManager = new GameObjectManager;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::start()
{
	timeSinceStart = glfwGetTime();
	while (!glfwWindowShouldClose(windowManager->window) && !quit)
	{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    ImGui_ImplGlfwGL3_NewFrame();
		glfwPollEvents();

    update();
    draw();

    glfwSwapBuffers(windowManager->window);
	}
  ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void Engine::internal_update()
{
  windowManager->update();

  renderer->viewport_width  = windowManager->viewport_width;
  renderer->viewport_height = windowManager->viewport_height;
  renderer->update();

  camera->aspectRatio = (f32)renderer->viewport_width / renderer->viewport_height;
  camera->updatePerspective();
}

void Engine::update()
{
  if (internal_settings_changed) internal_update();

  gameObjectManager->update();
}
void Engine::draw()
{
  gameObjectManager->draw(*camera);

#ifdef DEBUG
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("menu"))
    {
      ImGui::MenuItem("quit",                NULL, &quit);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("show"))
    {
      ImGui::MenuItem("engine info",         NULL, &show_engine_debug);
      ImGui::MenuItem("renderer info",       NULL, &show_renderer_debug);
      ImGui::MenuItem("resourceManager info",NULL, &show_resourceManager_debug);
      ImGui::MenuItem("windowmanager info",  NULL, &show_windowManager_debug);
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }


  if (show_resourceManager_debug)     resourceManager->display_debug_imGui();
  if (show_engine_debug)              display_debug_imGui();
  if (show_windowManager_debug)       windowManager->display_debug_imGui();
  if (show_renderer_debug)            renderer->display_debug_imGui();

  ImGui::Render();

#endif
}

void Engine::addGameObject(const char* handle, GameObject &gameobject)
{
	gameobject.id = (u32)gameObjectManager->gameObjects.size();
	gameObjectManager->gameObjects.emplace_back(std::make_unique<GameObject>(gameobject));
  resourceManager->addGameObject(handle, gameobject);
}

void Engine::display_debug_imGui()
{
  ImGui::Begin("Engine Debug Info");

  // Engine info
  ImGui::Text("%dfps %.3fms",(u32)ImGui::GetIO().Framerate, 1000.0f/ImGui::GetIO().Framerate);
  ImGui::Text("quit: %d", quit);
  ImGui::Text("deltaTime: %f", deltaTime);
  ImGui::Text("timeSinceStart: %f", timeSinceStart);
  ImGui::Text("currentFPS: %d", currentFPS);

  ImGui::Separator();

  static char buf1[256] = ""; ImGui::InputText("", buf1, 256);
  if (ImGui::Button("load mesh")) gameObjectManager->gameObjects[0]->mesh = resourceManager->mesh_isLoaded(buf1, buf1);
  ImGui::SameLine();
  if (ImGui::Button("load texture")) gameObjectManager->gameObjects[0]->texture.id = resourceManager->texture_isLoaded(buf1, buf1);
  ImGui::End();
}
