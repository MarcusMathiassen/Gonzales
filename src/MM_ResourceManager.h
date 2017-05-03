#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "MM_Typedefs.h"
#include "MM_GameObject.h"
#include "MM_Shader.h"
#include "MM_Texture.h"
#include "MM_Mesh.h"

struct ResourceManager
{
  std::map<string, u32> textures;
  std::map<string, Mesh>    meshes;

  void printAll();

  void addGameObject(const char* handle, const GameObject &gameobject);
  u32   texture_isLoaded(const char* handle, const char* file);
  Mesh  mesh_isLoaded(const char* handle, const char* file);
};
