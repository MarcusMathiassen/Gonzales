#pragma once

#include "MM_Utility.h"
#include "MM_Typedefs.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdio>

static void validateShader(const char *file, const char *type, u32 shader);
static void validateShaderProgram(const char* name, u32 shaderProgram);
static u32 createShader(const char *file, const GLenum type);

struct Shader
{
  enum { MVP, NUM_UNIFORMS };
  std::vector<s32> uniforms;
  u32 program{0};
  u32 uniform[NUM_UNIFORMS]{0};
  void bind();
  Shader(const std::string &file);
  Shader() = default;
};

static void validateShader(const char *file, const char *type, u32 shader)
{
  char  infoLog[512] = {0};
  s32     success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    printf("\nERROR::FILE %s\n", file);
    printf("ERROR::SHADER::%s::COMPILATION::FAILED\n\n%s", type, infoLog);
  }
}

static void validateShaderProgram(const char* name, u32 shaderProgram)
{
  char  infoLog[512] = {0};
  s32   success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
    printf("\nERROR::SHADER::PROGRAM::%s::LINKING::FAILED\n\n%s",name, infoLog);
  }
}

static u32 createShader(const char *file, const GLenum type)
{
  char *source = NULL;
  readFile(file, &source);

  u32 shader = glCreateShader(type);
  if (NULL != source)
  {
    glShaderSource(shader, 1, &source, NULL);
    free(source);
  }

  glCompileShader(shader);

  switch (type)
  {
    case GL_VERTEX_SHADER:    validateShader(file, "VERTEX",    shader); break;
    case GL_FRAGMENT_SHADER:  validateShader(file, "FRAGMENT",  shader); break;
    case GL_GEOMETRY_SHADER:  validateShader(file, "GEOMETRY",  shader); break;
  }
  printf("Shader loaded: %s\n", file);
  return shader;
}
