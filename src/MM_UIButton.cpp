#include "MM_UIButton.h"

UIButton::UIButton(float x, float y, float width, float height)
{
  transform.pos.x = x;
  transform.pos.y = y;

  const GLfloat positions[] =
  {
    0.0f,  height,
    0.0f,  0.0f,
    width, 0.0f,
    width,  height,
  };

  const GLfloat uv[] =
  {
    x,          y,
    x,          y + height,
    x + width,  y + height,
    x + width,  y,
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(NUM_BUFFERS, VBO);

  // POSITIONS
  glBindBuffer( GL_ARRAY_BUFFER, VBO[POSITION_VB] );
  glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, positions, GL_STATIC_DRAW );
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // TEXTURE COORDINATES
  glBindBuffer( GL_ARRAY_BUFFER, VBO[UV_VB] );
  glBufferData( GL_ARRAY_BUFFER, 4*sizeof(GLfloat)*2, uv, GL_STATIC_DRAW );
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // INDICES
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_VB] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLubyte), indices, GL_STATIC_DRAW);

  // SHADER SETUP
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
  const float inverseAspectRatio = 1.0f/aspectRatio;
  transform.scale = glm::vec3(inverseAspectRatio, 1, 0);
}

void UIButton::draw()
{
  glDisable(GL_DEPTH_TEST);

  glUseProgram(shaderProgram);
  texture.bind(0);

  glm::mat4 model = transform.getModel();
  glUniformMatrix4fv(uniform[MODEL_U], 1, GL_FALSE, &model[0][0]);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}
