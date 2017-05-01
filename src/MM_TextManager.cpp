#include "MM_TextManager.h"
#include "MM_Shader.h"

#define DIST_BETW_CHAR 0.030f

#define FONT_ATLAS_CHARACTERS_ROWS  16
#define FONT_ATLAS_CHARACTERS_COLS  16
#define FONT_ATLAS_CHARACTER_WIDTH  0.06125f
#define FONT_ATLAS_CHARACTER_HEIGHT 0.06125f

void TextManager::update()
{
  const size_t num_texts{text_buffer.size()};

  // Resize the vectors if text_buffer has increased
  if (num_texts > positions.size())
  {
    positions.resize(num_texts);
    colors.resize(num_texts);
    textcoord_indexes.resize(num_texts);
  }

  // Update the color and transform arrays
  size_t i {0};
  for (const auto &text : text_buffer)
  {
    const size_t num_chars{ text.str.length() };
    for (size_t j = 0; j < num_chars; ++j)
    {
      if (text.str[i] == ' ') continue;
      colors[i]   = text.color;
      positions[i]  = vec2(text.pos.x + j * FONT_ATLAS_CHARACTER_WIDTH, text.pos.y);
      textcoord_indexes[i++] = text.str[j];
    }
  }

  // POSITION_OFFSET BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITION_OFFSET]);
  // Does the buffer need to allocate more space?
  size_t position_bytes_needed {sizeof(vec2) * num_texts};
  if (position_bytes_needed > position_bytes_allocated) {
    glBufferData(GL_ARRAY_BUFFER, position_bytes_needed, &positions[0], GL_STREAM_DRAW);
    position_bytes_allocated = position_bytes_needed;
  }
  else
    glBufferSubData(GL_ARRAY_BUFFER, 0, position_bytes_allocated, &positions[0]);

  // COLOR BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOR]);
  // Does the buffer need to allocate more space?
  size_t color_bytes_needed {sizeof(vec4) * num_texts};
  if (color_bytes_needed > color_bytes_allocated)
  {
    glBufferData(GL_ARRAY_BUFFER, color_bytes_needed, &colors[0], GL_STREAM_DRAW);
    color_bytes_allocated = color_bytes_needed;
  }
  else
    glBufferSubData(GL_ARRAY_BUFFER, 0, color_bytes_allocated, &colors[0]);

    // TEXTCOORD BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTCOORD_INDEX]);
  // Does the buffer need to allocate more space?
  size_t textcoord_bytes_needed {sizeof(u8) * num_texts};
  if (textcoord_bytes_needed > textcoord_bytes_allocated) {
    glBufferData(GL_ARRAY_BUFFER, textcoord_bytes_needed, &textcoord_indexes[0], GL_STREAM_DRAW);
    textcoord_bytes_allocated = textcoord_bytes_needed;
  }
  else
    glBufferSubData(GL_ARRAY_BUFFER, 0, textcoord_bytes_allocated, &textcoord_indexes[0]);
}

void TextManager::drawAll()
{
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(VAO);
  glUseProgram(shaderProgram);
  texture.bind(0);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL, text_buffer.size());
  // for (const Text &text: text_buffer)
  // {
  //   const u8 num_chars{(u8)text.str.length()};
  //   for (u8 i = 0; i < num_chars; ++i)
  //   {
  //     if (text.str[i] == ' ') continue;
  //     glUniform2f(uniform[POSITION_OFFSET], text.pos.x + i * FONT_ATLAS_CHARACTER_WIDTH, text.pos.y);
  //     glUniform4f(uniform[COLOR], text.color.r, text.color.g, text.color.g, text.color.a);
  //     glUniform1i(uniform[TEXTCOORD_INDEX], text.str[i]);
  //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
  //   }
  // }
  glEnable(GL_DEPTH_TEST);
}

TextManager::TextManager() : texture("./res/font_VCR_OSD_MONO.bmp", GL_NEAREST)
{
  shaderProgram  = glCreateProgram();
  const u32 vs   = createShader("./res/MM_TextShader.vs", GL_VERTEX_SHADER);
  const u32 fs   = createShader("./res/MM_TextShader.fs", GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vs);
  glAttachShader(shaderProgram, fs);

  glBindAttribLocation(shaderProgram, 0, "pos_offset");
  glBindAttribLocation(shaderProgram, 1, "color");
  glBindAttribLocation(shaderProgram, 2, "textCoord_index");

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);
  validateShaderProgram("MM_TextManager", shaderProgram);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(NUM_BUFFERS, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDICES]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // uniform[POSITION_OFFSET] = glGetUniformLocation(shaderProgram, "pos_offset");
  // uniform[COLOR]           = glGetUniformLocation(shaderProgram, "color");
  // uniform[TEXTCOORD_INDEX] = glGetUniformLocation(shaderProgram, "textCoord_index");
}
TextManager::~TextManager()
{
  glDeleteVertexArrays(1, &VAO);
}
