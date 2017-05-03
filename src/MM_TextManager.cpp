#include "MM_TextManager.h"
#include "MM_Shader.h"

#define DIST_BETW_CHAR 0.030f

#define FONT_ATLAS_CHARACTERS_ROWS  16
#define FONT_ATLAS_CHARACTERS_COLS  16
#define FONT_ATLAS_CHARACTER_WIDTH  0.06125f
#define FONT_ATLAS_CHARACTER_HEIGHT 0.06125f

void TextManager::drawAll()
{
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(VAO);
  glUseProgram(shaderProgram);
  texture.bind(0);
  for (const auto &text: text_buffer)
  {
    glUniform4f(uniform[COLOR], text->color.r, text->color.g, text->color.g, text->color.a);
    const size_t num_chars{text->str.length()};
    for (size_t i = 0; i < num_chars; ++i)
    {
      if (text->str[i] == ' ') continue;
      glUniform2f(uniform[POSITION_OFFSET], text->pos.x + i * FONT_ATLAS_CHARACTER_WIDTH, text->pos.y);
      glUniform1i(uniform[TEXTCOORD_INDEX], text->str[i]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
    }
  }
  glEnable(GL_DEPTH_TEST);
}

TextManager::TextManager() : texture("./res/font_Consolas.bmp", GL_LINEAR)
{
  shaderProgram  = glCreateProgram();
  const u32 vs   = createShader("./res/MM_TextShader.vs", GL_VERTEX_SHADER);
  const u32 fs   = createShader("./res/MM_TextShader.fs", GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);
  validateShaderProgram("MM_TextManager", shaderProgram);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  u32 VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  uniform[POSITION_OFFSET] = glGetUniformLocation(shaderProgram, "pos_offset");
  uniform[COLOR]           = glGetUniformLocation(shaderProgram, "color");
  uniform[TEXTCOORD_INDEX] = glGetUniformLocation(shaderProgram, "textCoord_index");
}
TextManager::~TextManager()
{
  glDeleteVertexArrays(1, &VAO);
}
