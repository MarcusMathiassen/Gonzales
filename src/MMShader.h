#ifndef _MM_SHADER_H_
#define _MM_SHADER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include "MMUtility.h"

/* Declarations */
static void MMvalidateShader(const char *type, GLuint shader);
static void MMvalidateShaderProgram(GLuint shaderProgram);
static GLuint MMcreateShader(const char *file, const GLenum type);

/* Definitions */
static void MMvalidateShader(const char *type, GLuint shader)
{
  GLchar  infoLog[512] = {0};
  GLint   success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
    printf("ERROR::SHADER::%s::COMPILATION::FAILED\n\n%s", type, infoLog);
  }
}

static void MMvalidateShaderProgram(GLuint shaderProgram)
{
  GLchar  infoLog[512] = {0};
  GLint   success;
  glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING::FAILED\n\n%s", infoLog);
  }
}

static GLuint MMcreateShader(const char *file, const GLenum type)
{
  GLchar *source = NULL;
  MMreadFile(file, &source);

  GLuint shader = glCreateShader(type);

  if (source != NULL)
    free(source);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  switch (type)
  {
    case GL_VERTEX_SHADER:    MMvalidateShader("VERTEX",    shader); break;
    case GL_FRAGMENT_SHADER:  MMvalidateShader("FRAGMENT",  shader); break;
    case GL_GEOMETRY_SHADER:  MMvalidateShader("GEOMETRY",  shader); break;
  }

  return shader;
}

#endif