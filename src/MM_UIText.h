#pragma once

#include "MM_UIManager.h"
#include "MM_Typedefs.h"
#include <string>
#include <glm/glm.hpp>

struct Text : public UI
{
  u32 id{0};
  f32 x{0.0f}, y{0.0f};
  std::string str{"default text"};
  vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  Text() = default;
};
