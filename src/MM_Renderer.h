#pragma once

#include "MM_Typedefs.h"
#include "MM_GameObject.h"
#include <vector>

// @Note: only place where graphics code is run. ONLY place.

struct Renderer
{
  u32 viewport_width;
  u32 viewport_height;

  void update();
  void display_debug_imGui();
};
