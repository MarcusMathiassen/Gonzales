#ifndef _MM_TEXT_H_
#define _MM_TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "MM_Shader.h"
#include "MM_Texture.h"


#define MM_DIST_BETW_CHAR 0.030f
#define MM_CHAR_SIZE 0.0625f

struct MMCharacter;
struct MMTextBuffer;

static MMTextBuffer *MMDefaultTextBuffer = NULL;
static GLuint shaderProgram;
constexpr static GLubyte indices[]{0,1,2, 0,2,3};

static void mmDrawText(const char* text, float x, float y);

struct MMCharacter
{
  enum { POSITION, UV, INDEX, NUM_BUFFERS };
  GLuint VAO, VBO[NUM_BUFFERS];
  void draw() const
  {
    glBindVertexArray(VAO);
    glDrawElements( GL_TRIANGLES,     // type
                    4,                // num vertices
                    GL_UNSIGNED_BYTE, // indicies type
                    indices);         // pointer to indices
  }

  MMCharacter(float x = 0, float y = 0)
  {
    constexpr GLfloat positions[] = { 0.0f,         0.0f,         // lower left
                                      0.0f,         MM_CHAR_SIZE, // upper left
                                      MM_CHAR_SIZE, MM_CHAR_SIZE, // upper right
                                      0.0f,         MM_CHAR_SIZE  // lower right
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
    glBufferData( GL_ARRAY_BUFFER,
                  4*sizeof(GLfloat),
                  &positions[0],
                  GL_STATIC_DRAW
                );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // UV
    glBindBuffer( GL_ARRAY_BUFFER, VBO[UV] );
    glBufferData( GL_ARRAY_BUFFER,
                  4*sizeof(GLfloat),
                  &uv[0],
                  GL_STATIC_DRAW
                );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // INDEX
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  6*sizeof(GLubyte),
                  &indices[0],
                  GL_STATIC_DRAW);
  }
};

struct MMTextBuffer
{
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

    // ******************************************
    // CHARACTER SETUP
    // Using the ASCII code of each character up to 126
    // lets us do this: character['A' - 32] = 'A'
    // ******************************************
    // @CLEANUP: if it start with [32 + 16 * y*x] you'll get a cleaner syntax
    //        ala character['A'] = 'A'. Spares us a magic number.
    //        Need a bigger characterbuffer tho.
    for (uint8_t y = 0; y < 16; ++y)
      for (uint8_t x = 0; x < 16; ++x)
        character[16 * y+x] = MMCharacter(x*MM_CHAR_SIZE, y*MM_CHAR_SIZE);
  }
};

static void mmDrawText(const char* text, float x, float y)
{
  if (MMDefaultTextBuffer == NULL)
    MMDefaultTextBuffer = new MMTextBuffer();

  glUseProgram(shaderProgram);
  glDisable(GL_DEPTH_TEST);
  MMDefaultTextBuffer->texture.bind(0);
  const GLuint loc = glGetUniformLocation(shaderProgram, "pos");

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
