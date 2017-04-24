#ifndef _MM_SHADER_H_
#define _MM_SHADER_H_

#include "MM_Utility.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdlib>

/* Declarations */
static void validateShader(const char *file, const char *type, u32 shader);
static void validateShaderProgram(const char* name, u32 shaderProgram);
static u32 createShader(const char *file, const GLenum type);

/* Definitions */
static void validateShader(const char *file, const char *type, u32 shader)
{
  GLchar  infoLog[512] = {0};
  s32   success;
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
  GLchar  infoLog[512] = {0};
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
  GLchar *source = NULL;
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

#endif
