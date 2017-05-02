#pragma once

#include "MM_Typedefs.h"
#include "MM_Texture.h"
#include "MM_UIText.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include "MM_UIManager.h"

struct TextManager
{
  static constexpr u16 indices[]{0,1,2, 0,2,3};
  enum { POSITION_OFFSET, COLOR, TEXTCOORD_INDEX, NUM_UNIFORMS};
  u32           VAO;
  u32           shaderProgram;
  u32           uniform[NUM_UNIFORMS];
  Texture       texture;

  vector<Text>   text_buffer;
  void drawAll();
  TextManager();
  ~TextManager();
};
