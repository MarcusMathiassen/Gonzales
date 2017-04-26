#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "MM_GameObject.h"
#include "MM_Shader.h"
#include "MM_Texture.h"
#include "MM_Mesh.h"

struct ResourceManager
{
  std::vector<u32> shaders;
  std::vector<u32> meshes;
  std::vector<u32> textures;

  void printAll();

  void addGameObject(const GameObject &gameobject);
  void addShader(const Shader &shader);
  void addMesh(const Mesh &mesh);
  void addTexture(const Texture &texture);
};
