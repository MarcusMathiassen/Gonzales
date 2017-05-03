#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MM_Shader.h"
#include "MM_Texture.h"
#include "MM_Mesh.h"
#include "MM_Transform.h"
#include "MM_Camera.h"

#include <glm/glm.hpp>
#include <iostream>

struct GameObject
{
  u32 id      { 0 };
  enum        {MVP, NUM_UNIFORMS};
  //Shader      shader;
  Texture     texture;
  Mesh        mesh;
  Transform   transform;
  u32         shaderProgram{0};
  s32         uniform[NUM_UNIFORMS]{0};

  void update()
  {
    transform.rot.y = (f32)(glfwGetTime() * 0.5f);
  }
  void draw(const mat4 &viewProjection)
  {
    //shader.bind();
    glUseProgram(shaderProgram);
    texture.bind(0);

    mat4 mvp = viewProjection * transform.getModel();
    glUniformMatrix4fv(uniform[MVP], 1, GL_FALSE, &mvp[0][0]);

    mesh.draw();
  }

  GameObject() = default;
  GameObject(const char* file,
    const char* file_texture,
    const char* file_vertexShader,
    const char* file_fragmentShader) : mesh(file)
  {
    texture = Texture(file_texture, GL_LINEAR);

    shaderProgram      = glCreateProgram();
    u32 vertexShader   = createShader(file_vertexShader, GL_VERTEX_SHADER);
    u32 fragmentShader = createShader(file_fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "textCoord");
    glBindAttribLocation(shaderProgram, 2, "normal");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    validateShaderProgram("MMGameObject", shaderProgram);

    uniform[MVP] = glGetUniformLocation(shaderProgram, "MVP");
  }
};
