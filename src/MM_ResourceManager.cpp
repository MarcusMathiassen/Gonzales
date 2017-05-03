#include "MM_ResourceManager.h"
#include "MM_Utility.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void ResourceManager::printAll()
{
  ImGui::Begin("Resources");
  ImGui::Text("Textures:");
  for (const auto& map: textures)
  {
    ImGui::Text("name: %s", map.first.c_str());
  }
  ImGui::Separator();
  ImGui::Text("Meshes:");
  for (const auto& map: meshes)
  {
    ImGui::Text("name: %s", map.first.c_str());
  }
  ImGui::End();
}

void ResourceManager::addGameObject(const char* handle, const GameObject &gameobject)
{
  textures.insert(std::pair<string, u32>(handle, gameobject.texture.id));
  meshes.insert(std::pair<string, Mesh>(handle, gameobject.mesh));
}

u32 ResourceManager::texture_isLoaded(const char* handle, const char* file)
{
  auto it = textures.find(handle);
  if (it != textures.end())
    return it->second;
  Texture texture(file, GL_LINEAR); //@ HACK
  textures.insert(std::pair<string, u32>(handle, texture.id));
  return texture.id;
}
Mesh ResourceManager::mesh_isLoaded(const char* handle, const char* file)
{
  auto it = meshes.find(handle);
  if (it != meshes.end())
    return it->second;

  Mesh mesh(file);
  meshes.insert(std::pair<string, Mesh>(handle, mesh));
  return mesh;
}
