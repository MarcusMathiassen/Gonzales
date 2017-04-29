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
  std::map<std::string, unique_ptr<GameObject> > gameobjects;

  void printAll();

  void addGameObject(const char* handle, const unique_ptr<GameObject> &gameobject);
};
