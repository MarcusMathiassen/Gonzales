#pragma once

#include <vector>
#include <memory>

#include "MM_Typedefs.h"
#include "MM_Camera.h"

class UI
{
public:
  u32 id{0};
  static float aspectRatio;

  virtual void update(){};
  virtual void draw(){};
};

class UIManager
{
public:
  std::vector<std::unique_ptr<UI> > uiObjects;

  void update(const Camera &camera);
  void draw();

  UIManager() = default;
};
