#include "MM_WindowManager.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void WindowManager::createWindow(u32 _width, u32 _height, string _title)
{
  width   = _width;
  height  = _height;
  title   = _title;

  initGLFW();
  initGLEW();

  s32  count;
  auto modes  = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
  monitor_refresh_rate = modes->refreshRate;

  glfwSwapInterval(vsync);
}

void WindowManager::initGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  window = glfwCreateWindow(width,height,title.c_str(),NULL,NULL);
  glfwMakeContextCurrent(window);
}

void WindowManager::initGLEW()
{
  glewExperimental = true;
  glewInit();
}

void WindowManager::update()
{
  glfwSwapInterval(vsync);
  glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
  glfwGetWindowSize(window, &width, &height);
}

void WindowManager::display_debug_imGui()
{
  ImGui::Begin("WindowManager Debug Info");

  // Window info
  ImGui::Text("Title: %s", title.c_str());
  ImGui::Text("window: %dx%d", width, height);
  ImGui::Text("viewport: %dx%d", viewport_width, viewport_height);
  ImGui::Text("refresh rate: %d", monitor_refresh_rate);
  ImGui::Text("fullscreen: %d", fullscreen);
  ImGui::Checkbox("Vsync", (bool*)&vsync);


  ImGui::Separator();

  // OpenGL info
  ImGui::Text("OpenGL info");
  ImGui::Text("version: %s", glGetString(GL_VERSION));
  ImGui::Text("vendor: %s", glGetString(GL_VENDOR));
  ImGui::Text("renderer: %s", glGetString(GL_RENDERER));
  ImGui::End();
}
