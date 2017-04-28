#include "MM_TextManager.h"
#include "MM_Shader.h"

#define DIST_BETW_CHAR 0.030f

void TextManager::addText(const Text &text)
{
  text_buffer.emplace_back(text);
}
void TextManager::addText(const char* text, f32 x, f32 y, const vec4 &color)
{
  text_buffer.emplace_back(Text(text,x,y,color));
}

void TextManager::drawAll()
{
  glDisable(GL_DEPTH_TEST);
  glUseProgram(shaderProgram);
  texture.bind(0);
  glBindVertexArray(vao);
  for (const Text &text: text_buffer)
  {
    const u8 num_s8s{(u8)text.txt.length()};
    for (u8 i = 0; i < num_s8s; ++i)
    {
      if (text.txt[i] == ' ') continue;
      glUniform2f(uniform[POSITION_OFFSET], text.x + i * DIST_BETW_CHAR, text.y);
      glUniform1f(uniform[TEXTCOORD_INDEX], 'A');
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u8), indices, GL_STATIC_DRAW);

  uniform[POSITION_OFFSET] = glGetUniformLocation(shaderProgram, "pos_offset");
  uniform[TEXTCOORD_INDEX] = glGetUniformLocation(shaderProgram, "textCoord_index");

  // const f32 uv[] =
  // {
  //   x,                     y,
  //   x,                     y + MM_FONT_CHAR_SIZE,
  //   x + MM_FONT_CHAR_SIZE, y + MM_FONT_CHAR_SIZE,
  //   x + MM_FONT_CHAR_SIZE, y,
  // };

  // for (u8 y = 0; y < 16; ++y)
  //   for (u8 x = 0; x < 16; ++x)
  //     glyph[16 * y+x] = Glyph(x*MM_FONT_CHAR_SIZE, y*MM_FONT_CHAR_SIZE);
}
TextManager::~TextManager()
{
  glDeleteVertexArrays(1, &vao);
}
