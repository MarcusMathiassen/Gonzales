#pragma once

#include <vector>
#include <memory>

#include "MM_Typedefs.h"
#include "MM_Camera.h"

struct UI
{
  u32            id{0};
  static  f32  aspectRatio;

  virtual void update(){};
  virtual void draw(){};

  virtual ~UI() = default;
  UI() = default;
};

struct UIManager
{
  std::vector<std::unique_ptr<UI> > uiObjects;

  void update(const Camera& camera);
  void draw();

  UIManager() = default;
};
