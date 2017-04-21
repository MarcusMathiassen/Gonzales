#ifndef _MM_TEXT_H_
#define _MM_TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "MM_Shader.h"
#include "MM_Texture.h"

#include <cstring>

#define MM_DIST_BETW_CHAR 0.030f
#define MM_CHAR_SIZE 0.0625f

struct MMCharacter;
struct MMTextBuffer;

static MMTextBuffer *MMDefaultTextBuffer = NULL;
constexpr static GLubyte indices[]{0,1,2, 0,2,3};

static void mmDrawText(const char* text, float x, float y);

struct MMCharacter
{
  enum { POSITION, UV, INDEX, NUM_BUFFERS };
  GLuint VAO{0}, VBO[NUM_BUFFERS]{0};
  void draw() const
  {
    glBindVertexArray(VAO);
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
  }

  MMCharacter(float x = 0, float y = 0)
  {
    constexpr GLfloat positions[] = { 0.0f,         0.0f,
                                      0.0f,         MM_CHAR_SIZE,
                                      MM_CHAR_SIZE, MM_CHAR_SIZE,
                                      0.0f,         MM_CHAR_SIZE
                                    };
    const GLfloat uv[] =  { x,                y,
                            x,                y + MM_CHAR_SIZE,
                            x + MM_CHAR_SIZE, y + MM_CHAR_SIZE,
                            x + MM_CHAR_SIZE, y
                          };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(NUM_BUFFERS, VBO);

    // POSITION
    glBindBuffer( GL_ARRAY_BUFFER, VBO[POSITION] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat), positions, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
    // UV
    glBindBuffer( GL_ARRAY_BUFFER, VBO[UV] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat), uv, GL_STATIC_DRAW );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // INDEX
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);
  }
};

struct MMTextBuffer
{
  GLuint              shaderProgram;
  MMTexture           texture{"./res/MM_TextAtlas.png"};
  MMCharacter         character[256];
  MMTextBuffer()
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

    // Using the ASCII code of each character up to 126
    // lets us do this: character['A' - 32] = 'A'
    for (uint8_t y = 0; y < 16; ++y)
      for (uint8_t x = 0; x < 16; ++x)
        character[16 * y+x] = MMCharacter(x*MM_CHAR_SIZE, y*MM_CHAR_SIZE);
  }
};

static void mmDrawText(const char* text = "mmDrawText", float x = 0, float y = 0)
{
  if (MMDefaultTextBuffer == NULL)
    MMDefaultTextBuffer = new MMTextBuffer();

  //glDisable(GL_DEPTH_TEST);
  glUseProgram(MMDefaultTextBuffer->shaderProgram);
  MMDefaultTextBuffer->texture.bind(0);
  const GLuint loc = glGetUniformLocation(MMDefaultTextBuffer->shaderProgram, "pos");

  const size_t numChars = strlen(text);
  for (size_t i = 0; i < numChars; ++i)
  {
    if (text[i] == ' ') // dont draw space
      continue;
    glUniform2f(loc, x+MM_DIST_BETW_CHAR*i, y);
    MMDefaultTextBuffer->character[(int)text[i] - 32].draw();
  }
}

#endif
