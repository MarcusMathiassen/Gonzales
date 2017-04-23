#ifndef _MM_SHADER_H_
#define _MM_SHADER_H_

#include "MM_Utility.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdlib>

/* Declarations */
static void mmValidateShader(const char *file, const char *type, GLuint shader);
static void mmValidateShaderProgram(const char* name, GLuint shaderProgram);
static GLuint mmCreateShader(const char *file, const GLenum type);

/* Definitions */
static void mmValidateShader(const char *file, const char *type, GLuint shader)
{
  GLchar  infoLog[512] = {0};
  GLint   success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    printf("\nERROR::FILE %s\n", file);
    printf("ERROR::SHADER::%s::COMPILATION::FAILED\n\n%s", type, infoLog);
  }
}

static void mmValidateShaderProgram(const char* name, GLuint shaderProgram)
{
  GLchar  infoLog[512] = {0};
  GLint   success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
    printf("\nERROR::SHADER::PROGRAM::%s::LINKING::FAILED\n\n%s",name, infoLog);

  }
}

static GLuint mmCreateShader(const char *file, const GLenum type)
{
  GLchar *source = NULL;
  mmReadFile(file, &source);

  GLuint shader = glCreateShader(type);
  if (NULL != source)
  {
	  glShaderSource(shader, 1, &source, NULL);
	  free(source);
  }

  glCompileShader(shader);

  switch (type)
  {
    case GL_VERTEX_SHADER:    mmValidateShader(file, "VERTEX",    shader); break;
    case GL_FRAGMENT_SHADER:  mmValidateShader(file, "FRAGMENT",  shader); break;
    case GL_GEOMETRY_SHADER:  mmValidateShader(file, "GEOMETRY",  shader); break;
  }
  printf("Shader loaded: %s\n", file);
  return shader;
}

#endif
