#ifndef _MM_SHADER_H_
#define _MM_SHADER_H_

#include "MM_Utility.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <cstdlib>

/* Declarations */
static void MM_validateShader(const char *type, GLuint shader);
static void MM_validateShaderProgram(GLuint shaderProgram);
static GLuint MM_createShader(const char *file, const GLenum type);


/* Definitions */
static void MM_validateShader(const char *type, GLuint shader)
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

static void MM_validateShaderProgram(GLuint shaderProgram)
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

static GLuint MM_createShader(const char *file, const GLenum type)
{
  GLchar *source = NULL;
  MM_readFile(file, &source);

  GLuint shader = glCreateShader(type);

  if (source != NULL)
    free(source);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  switch (type)
  {
    case GL_VERTEX_SHADER:    MM_validateShader("VERTEX",    shader); break;
    case GL_FRAGMENT_SHADER:  MM_validateShader("FRAGMENT",  shader); break;
    case GL_GEOMETRY_SHADER:  MM_validateShader("GEOMETRY",  shader); break;
  }

  return shader;
}

#endif
