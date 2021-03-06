#include "MM_UIButton.h"

UIButton::UIButton(f32 x, f32 y, f32 width, f32 height, const char* file_texture) : texture(file_texture, GL_LINEAR)
{
  transform.pos.x = x;
  transform.pos.y = y;

  const f32 positions[] =
  {
    0.0f,  height,
    0.0f,  0.0f,
    width, 0.0f,
    width,  height,
  };

  const f32 uv[] =
  {
    x,          y,
    x,          y + height,
    x + width,  y + height,
    x + width,  y,
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  u32 vbo[NUM_BUFFERS];
  glGenBuffers(NUM_BUFFERS, vbo);

  glBindBuffer( GL_ARRAY_BUFFER, vbo[POSITION_VB] );
  glBufferData( GL_ARRAY_BUFFER, 4*sizeof(f32)*2, positions, GL_STATIC_DRAW );
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ARRAY_BUFFER, vbo[UV_VB] );
  glBufferData( GL_ARRAY_BUFFER, 4*sizeof(f32)*2, uv, GL_STATIC_DRAW );
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_VB] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);

  shaderProgram      = glCreateProgram();
  u32 vertexShader   = createShader("./res/MM_Text.vs", GL_VERTEX_SHADER);
  u32 fragmentShader = createShader("./res/MM_Text.fs", GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glBindAttribLocation(shaderProgram, 0, "position");
  glBindAttribLocation(shaderProgram, 1, "textCoord");

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);
  validateShaderProgram("UIButton", shaderProgram);

  uniform[MODEL_U] = glGetUniformLocation(shaderProgram, "model");
}

void UIButton::update()
{
  const f32 inverseAspectRatio = 1.0f/aspectRatio;
  transform.scale = vec3(inverseAspectRatio, 1, 0);
}

void UIButton::draw()
{
  glDisable(GL_DEPTH_TEST);

  glBindVertexArray(vao);
  glUseProgram(shaderProgram);
  texture.bind(0);

  mat4 model = transform.getModel();
  glUniformMatrix4fv(uniform[MODEL_U], 1, GL_FALSE, &model[0][0]);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}
