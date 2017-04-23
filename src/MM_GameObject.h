#ifndef _MM_GAMEOBJECT_H_
#define _MM_GAMEOBJECT_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include "MM_Shader.h"
#include "MM_Mesh.h"
#include "MM_Transform.h"
#include "MM_Camera.h"

#include <glm/glm.hpp>
#include <iostream>

struct MMGameObject
{
  enum {POSITION, TRANSFORM, NUM_UNIFORMS};
  MMMesh        mesh;
  MMTransform   transform;
  GLuint        shaderProgram{0};
  GLint         uniform[NUM_UNIFORMS]{0};

  void draw()
  {
    glUseProgram(shaderProgram);
    glm::mat4 model = mmGetCamera().getViewProjection() * transform.getModel();
    glUniformMatrix4fv(uniform[TRANSFORM], 1, GL_FALSE, &model[0][0]);
    mesh.draw();
  }

  MMGameObject(const char* file,
    const char* file_vertexShader,
    const char* file_fragmentShader) : mesh(file)
  {
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

    uniform[TRANSFORM] = glGetUniformLocation(shaderProgram, "transform");
  }
};

#endif
