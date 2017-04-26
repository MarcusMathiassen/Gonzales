#include "MM_ResourceManager.h"

void ResourceManager::printAll()
{
  std::cout << "\n\n\n\n\n";
  std::cout << "-::::::Resource Manager::::::-\n";
  std::cout << "------SHADERS IDS:------\n";
  for (const auto& shader: shaders)
    std::cout << "------: " << shader << '\n';

  std::cout << "------MESHES IDS:------\n";
  for (const auto& mesh: meshes)
    std::cout << "------: " << mesh << '\n';

  std::cout << "------TEXTURES IDS:------\n";
  for (const auto& texture: textures)
    std::cout << "------: " << texture << '\n';
}

  void ResourceManager::addGameObject(const GameObject &gameobject)
  {
    shaders.emplace_back(gameobject.shader.program);
    meshes.emplace_back(gameobject.mesh.VAO);
    textures.emplace_back(gameobject.texture.id);
  }

  void ResourceManager::addShader(const Shader &shader)
  {
    shaders.emplace_back(shader.program);
  }
  void ResourceManager::addMesh(const Mesh &mesh)
  {
    meshes.emplace_back(mesh.VAO);
  }

  void ResourceManager::addTexture(const Texture &texture)
  {
    textures.emplace_back(texture.id);
  }

