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
  //enum { POSITION_OFFSET, COLOR, TEXTCOORD_INDEX, NUM_UNIFORMS};
  enum { POSITION_OFFSET, COLOR, TEXTCOORD_INDEX, INDICES, NUM_BUFFERS};
  u32           VAO;
  u32           VBO[NUM_BUFFERS];
  u32           shaderProgram;
  //u32           uniform[NUM_UNIFORMS];
  Texture       texture;

  size_t        position_bytes_allocated{0};
  size_t        color_bytes_allocated{0};
  size_t        textcoord_bytes_allocated{0};

  vector<Text>   text_buffer;
  vector<vec2>   positions;
  vector<vec4>   colors;
  vector<u8>     textcoord_indexes;
  void update();
  void drawAll();
  TextManager();
  ~TextManager();
};
