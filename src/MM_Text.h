#ifndef _MM_TEXT_H_
#define _MM_TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "MM_Shader.h"
#include "MM_Texture.h"
#include "MM_Camera.h"
#include "MM_Transform.h"

#include <type_traits>
#include <iostream>
#include <cstring>

#define FONT_ATLAS_CHARACTERS_ROWS  16
#define FONT_ATLAS_CHARACTERS_COLS  16
#define FONT_ATLAS_CHARACTER_WIDTH  0.06125f
#define FONT_ATLAS_CHARACTER_HEIGHT 0.06125f

struct MMCharacter;
struct MMTextBuffer;
static MMTextBuffer *MMDefaultTextBuffer = NULL;

static void drawText(const char* text, f32 x, f32 y, f32 aspectRatio);

struct MMCharacter
{
  static constexpr GLubyte indices[]{0,1,2, 0,2,3};
  enum { POSITION, UV, INDEX, NUM_BUFFERS };
  u32 VAO{0}, VBO[NUM_BUFFERS]{0};
  void draw() const
  {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
  }

  MMCharacter(f32 x = 0, f32 y = 0)
  {
    constexpr f32 positions[] =
    {
      0.0f,                       FONT_ATLAS_CHARACTER_HEIGHT,
      0.0f,                       0.0f,
      FONT_ATLAS_CHARACTER_WIDTH, 0.0f,
      FONT_ATLAS_CHARACTER_WIDTH, FONT_ATLAS_CHARACTER_HEIGHT,
    };

    const f32 uv[] =
    {
      x,                              y,
      x,                              y + FONT_ATLAS_CHARACTER_HEIGHT,
      x + FONT_ATLAS_CHARACTER_WIDTH, y + FONT_ATLAS_CHARACTER_HEIGHT,
      x + FONT_ATLAS_CHARACTER_WIDTH, y,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(NUM_BUFFERS, VBO);

    glBindBuffer( GL_ARRAY_BUFFER, VBO[POSITION] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(f32)*2, positions, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ARRAY_BUFFER, VBO[UV] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(f32)*2, uv, GL_STATIC_DRAW );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);
  }
};

struct MMTextBuffer
{
  enum {MODEL, NUM_UNIFORMS};
  u32                 shaderProgram;
  Texture             texture;
  MMCharacter         character[256];
  s32                 uniform[NUM_UNIFORMS];

  MMTextBuffer(const char* fontAtlas, f32 filtering) : texture{fontAtlas, filtering}
  {
    shaderProgram = glCreateProgram();
    u32 vertexShader   = createShader("./res/MM_Text.vs", GL_VERTEX_SHADER);
    u32 fragmentShader = createShader("./res/MM_Text.fs", GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "textCoord");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    validateShaderProgram("MMText", shaderProgram);

    uniform[MODEL] = glGetUniformLocation(shaderProgram, "model");

    for (u8 y = 0; y < FONT_ATLAS_CHARACTERS_ROWS; ++y)
      for (u8 x = 0; x < FONT_ATLAS_CHARACTERS_COLS; ++x)
        character[FONT_ATLAS_CHARACTERS_ROWS * y+x] = MMCharacter(x*FONT_ATLAS_CHARACTER_WIDTH, y*FONT_ATLAS_CHARACTER_HEIGHT);
  }
  ~MMTextBuffer()
  {
    glDeleteProgram(shaderProgram);
  }
};

static void drawText(const char* text, f32 x, f32 y, f32 aspectRatio)
{
  if (MMDefaultTextBuffer == NULL)
    MMDefaultTextBuffer = new MMTextBuffer("./res/font_VCR_OSD_MONO.bmp", GL_NEAREST);

  glDisable(GL_DEPTH_TEST);
  glUseProgram(MMDefaultTextBuffer->shaderProgram);
  MMDefaultTextBuffer->texture.bind(0);

  const f32 inverseAspectRatio = 1.0f/aspectRatio;
  Transform transform;
  transform.pos = vec3(x,y,0);
  transform.scale = vec3(inverseAspectRatio, 1, 0);

  const auto numChars = strlen(text);
  for (size_t i = 0; i < numChars; ++i)
  {
    if (text[i] == ' ') continue;

    transform.pos.x = x+(FONT_ATLAS_CHARACTER_WIDTH)*i*inverseAspectRatio;
    mat4 model = transform.getModel();
    glUniformMatrix4fv(MMDefaultTextBuffer->uniform[0], 1, GL_FALSE, &model[0][0]);

    MMDefaultTextBuffer->character[(int)text[i] - 32].draw();
  }
}

#endif
