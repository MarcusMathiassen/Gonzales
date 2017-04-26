#include "MM_Shader.h"

#include <string>

Shader::Shader(const std::string &file)
{
  program = glCreateProgram();
  u32 vs  = createShader((file + ".vs").c_str(), GL_VERTEX_SHADER);
  u32 fs  = createShader((file + ".fs").c_str(), GL_FRAGMENT_SHADER);

  glAttachShader(program, fs); glDeleteShader(fs);
  glAttachShader(program, vs); glDeleteShader(vs);

  glBindAttribLocation(program, 0, "position");
  glBindAttribLocation(program, 1, "textCoord");
  glBindAttribLocation(program, 2, "normal");

  glLinkProgram(program);
  glValidateProgram(program);
  validateShaderProgram("MM_Shader", program);

  uniform[MVP] = glGetUniformLocation(program, "MVP");
}

void Shader::bind()
{
  glUseProgram(program);
}
