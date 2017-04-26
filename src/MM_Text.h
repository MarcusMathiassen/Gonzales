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

#define MM_DIST_BETW_CHAR 0.030f
#define MM_FONT_CHAR_SIZE 0.0625f

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
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
  }

  MMCharacter(f32 x = 0, f32 y = 0)
  {
    constexpr f32 positions[] =
    {
      0.0f,                 MM_FONT_CHAR_SIZE,
      0.0f,                 0.0f,
      MM_FONT_CHAR_SIZE,    0.0f,
      MM_FONT_CHAR_SIZE,    MM_FONT_CHAR_SIZE,
    };

    const f32 uv[] =
    {
      x,                     y,
      x,                     y + MM_FONT_CHAR_SIZE,
      x + MM_FONT_CHAR_SIZE, y + MM_FONT_CHAR_SIZE,
      x + MM_FONT_CHAR_SIZE, y,
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

    for (u8 y = 0; y < 16; ++y)
      for (u8 x = 0; x < 16; ++x)
        character[16 * y+x] = MMCharacter(x*MM_FONT_CHAR_SIZE, y*MM_FONT_CHAR_SIZE);
  }
  ~MMTextBuffer()
  {
    glDeleteProgram(shaderProgram);
  }
};

static void drawText(const char* text, f32 x, f32 y, f32 aspectRatio)
{
  if (MMDefaultTextBuffer == NULL)
    MMDefaultTextBuffer = new MMTextBuffer("./res/MM_fontAtlas.png", GL_LINEAR);

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

    transform.pos.x = x+MM_DIST_BETW_CHAR*i*inverseAspectRatio;
    mat4 model = transform.getModel();
    glUniformMatrix4fv(MMDefaultTextBuffer->uniform[0], 1, GL_FALSE, &model[0][0]);

    MMDefaultTextBuffer->character[(int)text[i] - 32].draw();
  }
}

#endif
