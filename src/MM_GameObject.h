#ifndef _MM_GAMEOBJECT_H_
#define _MM_GAMEOBJECT_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include "MM_Shader.h"
#include "MM_Texture.h"
#include "MM_Mesh.h"
#include "MM_Transform.h"
#include "MM_Camera.h"

#include <glm/glm.hpp>
#include <iostream>

struct MMGameObject
{
  enum {MVP, NUM_UNIFORMS};
  MMTexture     texture;
  MMMesh        mesh;
  MMTransform   transform;
  GLuint        shaderProgram{0};
  GLint         uniform[NUM_UNIFORMS]{0};

  void draw()
  {
    texture.bind(0);
    glUseProgram(shaderProgram);

    glm::mat4 mvp = mmMainCamera->getViewProjection() * transform.getModel();
    glUniformMatrix4fv(uniform[MVP], 1, GL_FALSE, &mvp[0][0]);

    mesh.draw();
  }

  MMGameObject(const char* file,
    const char* file_texture,
    const char* file_vertexShader,
    const char* file_fragmentShader) : mesh(file)
  {
    texture = MMTexture(file_texture, GL_LINEAR);

    shaderProgram         = glCreateProgram();
    GLuint vertexShader   = mmCreateShader(file_vertexShader, GL_VERTEX_SHADER);
    GLuint fragmentShader = mmCreateShader(file_fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "textCoord");
    glBindAttribLocation(shaderProgram, 2, "normal");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    mmValidateShaderProgram("MMGameObject", shaderProgram);

    uniform[MVP] = glGetUniformLocation(shaderProgram, "MVP");
  }
};

#endif
