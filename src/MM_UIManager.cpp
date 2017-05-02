#include "MM_UIManager.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

f32 UI::aspectRatio{1.0f};

void UIManager::update(const Camera &camera)
{
  UI::aspectRatio = camera.aspectRatio;

  for (auto& ui : uiObjects)
    ui->update();
}

void UIManager::draw()
{
  for (auto &ui : uiObjects)
    ui->draw();


  ImGui_ImplGlfwGL3_NewFrame();

  {
      ImGui::Text("Hello, form UIManager!");
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Button("Test button");
  }

  ImGui::Render();
}
