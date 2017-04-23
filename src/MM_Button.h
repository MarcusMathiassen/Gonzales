#ifndef _MM_BUTTON_H_
#define _MM_BUTTON_H_

#include "MM_Shader.h"
#include "MM_Transform.h"
#include "MM_Camera.h"
#include "MM_Texture.h"

struct MMButton
{
  static constexpr GLubyte indices[]{0,1,2, 0,2,3};

  enum { POSITION_VB, UV_VB, INDEX_VB, NUM_BUFFERS };
  enum { MODEL_U, NUM_UNIFORMS};

  MMTexture           texture;
  GLuint              shaderProgram;
  MMTransform         transform;
  GLuint              VAO{0}, VBO[NUM_BUFFERS]{0};
  GLint               uniform[NUM_UNIFORMS];


  MMButton( float x = 0, float y = 0,
            float buttonWidth = 0.2, float buttonHeight = 0.1,
            const char* file_texture = "./res/marble.jpg") : texture(file_texture, GL_LINEAR)
  {
    const GLfloat positions[] =
    {
      0.0f,                 buttonHeight,
      0.0f,                 0.0f,
      buttonWidth,          0.0f,
      buttonWidth,          buttonHeight,
    };

    const GLfloat uv[] =
    {
      x,                     y,
      x,                     y + buttonHeight,
      x + buttonWidth,       y + buttonHeight,
      x + buttonWidth,       y,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(NUM_BUFFERS, VBO);

    // positions
    glBindBuffer( GL_ARRAY_BUFFER, VBO[POSITION_VB] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, positions, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // texture coordinates
    glBindBuffer( GL_ARRAY_BUFFER, VBO[UV_VB] );
    glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, uv, GL_STATIC_DRAW );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // indices
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_VB] );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);

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
    mmValidateShaderProgram("MMButton", shaderProgram);

    uniform[MODEL_U] = glGetUniformLocation(shaderProgram, "model");
  }
  void draw() const
  {
    glDisable(GL_DEPTH_TEST);
    glUseProgram(shaderProgram);
    glm::mat4 model = transform.getModel();
    glUniformMatrix4fv(uniform[MODEL_U], 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
  }
};

#endif
