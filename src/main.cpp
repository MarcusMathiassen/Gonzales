
#include "MM.h"

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

MMApp app;
MMGameObject *cube;
int main()
{
  app.openGLVersion = 4.1f;
  app.width = 1024;
  app.height = 1024;
  mmInit(app);

  cube = new MMGameObject(      "./res/cubeUV.obj",
                                "./res/marble.jpg",
                                "./res/MM_BasicGameObject.vs",
                                "./res/MM_BasicGameObject.fs");
  mmStart(app);
  delete cube;
}

char buffer[50];
void draw()
{
  cube->transform.rot.z = glfwGetTime()*0.5;
  cube->transform.rot.y = glfwGetTime()*0.3;
  cube->transform.rot.x = glfwGetTime();

  cube->draw();

  mmDrawText("Hello, this is a string literal", -1.0f, 0.9f);
  sprintf (buffer, "Hello, this is a char[]: %dfps %0.3fms", app.currentFPS, (float)app.deltaTime);
  mmDrawText(buffer, -1.0f, 0.8f);
  std::stringstream ss;
  ss << "Hello, this is a std::string: " << app.currentFPS << "fps " << (float)app.deltaTime << "ms";
  mmDrawText(ss.str(), -1.0f, 0.7f);
}
