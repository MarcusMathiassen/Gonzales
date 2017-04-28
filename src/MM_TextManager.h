#pragma once

#include "MM_Typedefs.h"
#include "MM_Texture.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

struct Text
{
  u32 id{0};
  f32 x{0.0f}, y{0.0f};
  std::string str{"default text"};
  vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
  Text() = default;
};

struct TextManager
{
  static constexpr u16 indices[]{0,1,2, 0,2,3};
  enum { POSITION_OFFSET, COLOR, TEXTCOORD_INDEX, NUM_UNIFORMS};
  u32           vao;
  u32           shaderProgram;
  u32           uniform[NUM_UNIFORMS];
  Texture       texture;

  vector<Text> text_buffer;
  void drawAll();
  TextManager();
  ~TextManager();
};
