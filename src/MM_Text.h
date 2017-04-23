#ifndef _MM_TEXT_H_
#define _MM_TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "MM_Shader.h"
#include "MM_Texture.h"

#include <type_traits>
#include <iostream>
#include <cstring>

#define MM_DIST_BETW_CHAR 0.030f
#define MM_FONT_CHAR_SIZE 0.0625f

/* Declarations */
struct MMCharacter;
struct MMTextBuffer;
static MMTextBuffer *MMDefaultTextBuffer = NULL;

template<typename T>
static void mmDrawText(const T& t, float x, float y);

/* Definitions */
struct MMCharacter
{
  static constexpr GLubyte indices[]{0,1,2, 0,2,3};
  enum { POSITION, UV, INDEX, NUM_BUFFERS };
  GLuint VAO{0}, VBO[NUM_BUFFERS]{0};
  void draw() const
  {
    glBindVertexArray(VAO);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
  }

  MMCharacter(float x = 0, float y = 0)
  {
    constexpr GLfloat positions[] =
    {
      0.0f,                 MM_FONT_CHAR_SIZE,
      0.0f,                 0.0f,
      MM_FONT_CHAR_SIZE,    0.0f,
      MM_FONT_CHAR_SIZE,    MM_FONT_CHAR_SIZE,
    };

    const GLfloat uv[] =
    {
      x,                     y,
      x,                     y + MM_FONT_CHAR_SIZE,
      x + MM_FONT_CHAR_SIZE, y + MM_FONT_CHAR_SIZE,
      x + MM_FONT_CHAR_SIZE, y,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(NUM_BUFFERS, VBO);

    // POSITION
    glBindBuffer( GL_ARRAY_BUFFER, VBO[POSITION] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, positions, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // UV
    glBindBuffer( GL_ARRAY_BUFFER, VBO[UV] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, uv, GL_STATIC_DRAW );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // INDEX
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);
  }
};

struct MMTextBuffer
{
  enum {POSITION, TEXTCOORD, NUM_UNIFORMS};
  GLuint              shaderProgram;
  MMTexture           texture;
  MMCharacter         character[256];
  GLint               uniform[NUM_UNIFORMS];

  MMTextBuffer(const char* fontAtlas, GLfloat filtering) : texture{fontAtlas, filtering}
  {
    shaderProgram = glCreateProgram();
    GLuint vertexShader   = mmCreateShader("./res/MM_Text.vs", GL_VERTEX_SHADER);
    GLuint fragmentShader = mmCreateShader("./res/MM_Text.fs", GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "textCoord");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    mmValidateShaderProgram("MMText", shaderProgram);

    uniform[POSITION] = glGetUniformLocation(shaderProgram, "pos");

    // Using the ASCII code of each character up to 126
    // lets us do this: character['A' - 32] = 'A'
    for (uint8_t y = 0; y < 16; ++y)
      for (uint8_t x = 0; x < 16; ++x)
        character[16 * y+x] = MMCharacter(x*MM_FONT_CHAR_SIZE, y*MM_FONT_CHAR_SIZE);
  }
  ~MMTextBuffer()
  {
    glDeleteProgram(shaderProgram);
  }
};

template<typename T>
static void mmDrawText(const T& t, float x, float y)
{
  if (MMDefaultTextBuffer == NULL)
    MMDefaultTextBuffer = new MMTextBuffer("./res/MM_fontAtlas.png", GL_LINEAR);

  glDisable(GL_DEPTH_TEST);
  glUseProgram(MMDefaultTextBuffer->shaderProgram);
  MMDefaultTextBuffer->texture.bind(0);

  std::string text;
#ifdef _WIN32 // win32 doesnt support if constexpr yet. Also doesnt support the needed overloads for std::to_string
    if (std::is_same<std::string, T>::value)            text = t;
    else if (std::is_array<T>::value)                   text = t;
#else // everyone else supports it
    if constexpr (std::is_same<std::string, T>::value)  text = t;
    else if constexpr (std::is_array<T>::value)         text = t;
	  else text = std::to_string(t);
#endif

  const auto numChars = text.length();
  const GLuint position_loc = MMDefaultTextBuffer->uniform[0]; // 0 should be POSITION. For some reason it doesnt see it.
  for (size_t i = 0; i < numChars; ++i)
  {
    if (text[i] == ' ') continue;
    glUniform2f(position_loc, x+MM_DIST_BETW_CHAR*i, y);
    MMDefaultTextBuffer->character[(int)text[i] - 32].draw();
  }
}

#endif
