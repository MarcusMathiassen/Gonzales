#include "MM_TextManager.h"
#include "MM_Shader.h"

#define DIST_BETW_CHAR 0.030f

void TextManager::drawAll()
{
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(vao);
  glUseProgram(shaderProgram);
  texture.bind(0);
  for (const Text &text: text_buffer)
  {
    const u8 num_s8s{(u8)text.txt.length()};
    for (u8 i = 0; i < num_s8s; ++i)
    {
      if (text.txt[i] == ' ') continue;
      glUniform2f(uniform[POSITION_OFFSET], text.x + i * DIST_BETW_CHAR, text.y);
      glUniform1i(uniform[TEXTCOORD_INDEX], text.txt[i]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
  }
  glEnable(GL_DEPTH_TEST);
}

TextManager::TextManager() : texture("./res/MM_fontAtlas.png", GL_LINEAR)
{
  shaderProgram  = glCreateProgram();
  const u32 vs   = createShader("./res/MM_TextShader.vs", GL_VERTEX_SHADER);
  const u32 fs   = createShader("./res/MM_TextShader.fs", GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);
  validateShaderProgram("MM_TextManager", shaderProgram);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  u32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  uniform[POSITION_OFFSET] = glGetUniformLocation(shaderProgram, "pos_offset");
  uniform[TEXTCOORD_INDEX] = glGetUniformLocation(shaderProgram, "textCoord_index");
}
TextManager::~TextManager()
{
  glDeleteVertexArrays(1, &vao);
}
