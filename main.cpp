
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "MM.h"

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(1024, 1024, "OpenGLTest", NULL, NULL);
	glfwMakeContextCurrent(window);

  glewExperimental = true;

	glewInit();

  glClearColor(1.0f, 0.3f, 0.3f, 1.0f);

  glfwSwapInterval(1);

  int hue = 0;
  while ( !glfwWindowShouldClose(window)
      &&  !(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
  {
    if (hue++ > 360) hue = 0;
    glm::vec3 col = MMHSVtoRGB(hue, 1.0, 1.0);
    glClearColor(col.r,col.g,col.b,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    glfwSwapBuffers(window);
  }
  glfwTerminate();
}
