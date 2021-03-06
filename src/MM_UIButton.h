#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "MM_UIManager.h"
#include "MM_Transform.h"
#include "MM_Texture.h"
#include "MM_Shader.h"
#include "MM_TextManager.h"

class UIButton : public UI
{
  static constexpr u16 indices[]{0,1,2, 0,2,3};
  enum { POSITION_VB, UV_VB, INDEX_VB, NUM_BUFFERS };
  enum { MODEL_U, NUM_UNIFORMS };


public:
  Transform     transform;
  Text          text;
  Texture       texture;
  u32           shaderProgram;
  u32           vao{0};
  s32           uniform[NUM_UNIFORMS];

  f32 width{0.2f}, height{0.1f};

  void update();
  void draw();

  UIButton() = default;
  UIButton(f32 x, f32 y, f32 width, f32 height, const char* file_texture);

};
