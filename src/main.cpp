
#include "MM.h"

int main()
{
  auto window = MM_newWindow(512, 512, "App", 4.1);

  GLuint shaderProgram  = glCreateProgram();
  GLuint vertexShader   = MM_createShader("./res/basic.vs", GL_VERTEX_SHADER);
  GLuint fragmentShader = MM_createShader("./res/basic.fs", GL_FRAGMENT_SHADER);

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glBindAttribLocation(shaderProgram, 0, "position");

  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);

  GLfloat vertices[] =
  {
    -0.5, -0.5,
     0.0,  0.5,
     0.5, -0.5,
  };

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData( GL_ARRAY_BUFFER,
                sizeof(vertices) * sizeof(GLfloat),
                vertices,
                GL_STATIC_DRAW
              );

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  int hue = 0;
  while ( !glfwWindowShouldClose(window)
      &&  !(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
  {
    if (hue++ > 360)
      hue = 0;

    glm::vec3 color = MM_HSVtoRGB(hue, 1.0, 1.0);
    glClearColor(color.r,color.g,color.b,1.0);

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // OpenGL fixed pipeline <3.3version
    // glColor3f(1,1,1);
    // glBegin(GL_LINES);
    // glVertex2f(-1,-1);
    // glVertex2f(1, 1);
    // glEnd();

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
