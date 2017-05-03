#include "MM_Renderer.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void Renderer::update()
{
  glViewport(0, 0, viewport_width, viewport_height);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Renderer::display_debug_imGui()
{
  ImGui::Begin("Renderer Debug Info");
  ImGui::Text("viewport: %dx%d", viewport_width, viewport_height);
  ImGui::End();
}
